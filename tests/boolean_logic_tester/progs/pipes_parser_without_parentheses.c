/* This program implements parsing of a basic user prompt
 * containing pipes without parentheses. For simplicity,
 * it launches programs located in the current directory.
 * Each program is named with a single letter of the English
 * alphabet. Lowercase-letter programs always return 0 (success),
 * while uppercase-letter programs always return 1 (failure) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#define EXIT_CMD		"exit"
#define MAX_PIPES_NUM	128
#define MAX_OPS_NUM		128
#define READ_END		0
#define WRITE_END		1
#define DEFAULT_FD      -1

/* STDIN_FILENO always must be bonded with read-end;
 * STDOUT_FILENO always must be bonded with write-end */
typedef struct s_operand
{
	char	name[2]; // path to program
	int		read_end; // stdin
	int		write_end; // stdout
}	t_operand;

int	main(void)
{
	char prompt[] = "dchernik@c3r3s6: ";
	char *rline_buf = NULL;

	while (1) // readline loop
	{
		rline_buf = readline(prompt);
		/*printf("rline_buf = %p\n", rline_buf);
		printf("rline_buf = \"%s\"\n", rline_buf);*/
		if (strlen(rline_buf) == 0)
		{
			printf("rline_buf is NULL\n");
			free(rline_buf);
			rline_buf = NULL;
			continue;
		}
		add_history(rline_buf);
		if (!strncmp(rline_buf, EXIT_CMD, strlen(EXIT_CMD)))
		{
			free(rline_buf);
			rline_buf = NULL;
			break;
		}

		// Let's analyze received prompt/request
		size_t		i; // First auxiliary index
	
		size_t		pi; // Prompt index
		size_t		prompt_len;
		char		*prompt;

		size_t		pipe_cnt;
		int			pipes[MAX_PIPES_NUM][2];

		size_t		op_cnt;	// Operand counter
		t_operand	ops[MAX_OPS_NUM]; // operands (programs to launch)

        bool        f_err = false;

        i = 0;
        while (i < MAX_OPS_NUM)
        {
            ops[i].write_end = DEFAULT_FD;
            ops[i].read_end = DEFAULT_FD;
            ++i;
        }
		prompt = rline_buf;
		prompt_len = strlen(prompt);
        pi = 0;
		op_cnt = 0;
		pipe_cnt = 0;
		while (pi < prompt_len)
		{
			while (prompt[pi] == ' ' && pi < prompt_len)
				++pi;
			if (pi == prompt_len)
				break;
			if (isalpha(prompt[pi]))
			{
				ops[op_cnt].name[0] = prompt[pi];
				ops[op_cnt].name[1] = '\0';
                ++pi;
                // Let's see what's goes next
                while (prompt[pi] == ' ' && pi < prompt_len)
                    ++pi;
                if (pi == prompt_len) // It means nothing is on the rights (just spaces)
                {
                    if (pipe_cnt > 0)
                    {
                        ops[op_cnt].read_end = pipe_cnt;
                        ops[op_cnt].write_end = -1;
                    }
                    ++op_cnt;
                    break;
                }

                if (prompt[pi] == '|') // If further goes pipe
                {
                    // Let's create a pipe
                    if (pipe(&pipes[pipe_cnt][0]) == -1)
                    {
                        fprintf(stderr, "Can't create pipe: %s\n", strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    if (pipe_cnt == 0) // If it's the first operand found
                        ops[op_cnt].read_end = -1;
                    else // It's not the first operand
                        ops[op_cnt].read_end = pipe_cnt - 1;

                    ops[op_cnt].write_end = pipe_cnt;

                    ++pipe_cnt;
                }
                else // if (prompt[pi] != '|')
                {
                    f_err = true;
                    if (isalpha(prompt[pi]))
                    {
                        fprintf(stderr, "Parsing error: "
                            "After operand cannot go another operand\n");
                    }
                    else
                    {
				        fprintf(stderr, "Parsing error. What is '%c' ?\n",
                            prompt[pi]);
                    }
                    break;
                }

                ++op_cnt;

			} // if ((prompt[pi] >= 'a' && prompt[pi] <= 'z')
            else
            {
                f_err = true;
				fprintf(stderr, "Parsing error. What is '%c' ?\n", prompt[pi]);
				break;
            }
			++pi;
		} // while (pi < prompt_len)

        if (f_err) // In case of non-critical parsing error
            continue;

        // Let's output the pipes we found
        i = 0; 
        while (i < pipe_cnt)
        {
            printf("%lu: [%d] [%d]\n", i + 1,
                pipes[i][READ_END], pipes[i][WRITE_END]);
            ++i;
        }
        printf("\n");

        // Let's output the operators we found
        i = 0; 
        while (i < op_cnt)
        {
            printf("%lu: [%s] [%d] [%d]\n", i + 1,
                ops[i].name, ops[i].read_end, ops[i].write_end);
            ++i;
        }
        printf("\n");

        // Now we have to launch all operand-programs
        pid_t   progs[MAX_OPS_NUM];
        int     op_i; // Operand index

        // Go from right to left operand
        op_i = op_cnt - 1;
        while (op_i >= 0)
        {
            progs[op_i] = fork();
            if (progs[op_i] == -1)
            {
                fprintf(stderr, "Can't fork: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            else if (progs[op_i] == 0) // 0 always is returned in the child
            {
                // We're in the new process
                char    *op_argv[2] = { 0, 0 };
                char    *envp[] = { "HOME=/home/user",
                                    "PATH=/bin:/usr/bin",
                                    "USER=user", 0 };

                op_argv[0] = &ops[op_i].name[0];

                // Let's attach pipes to each process (operand)
                if (ops[op_i].write_end != -1)
                    dup2(pipes[ops[op_i].write_end][WRITE_END], STDOUT_FILENO);

                if (ops[op_i].read_end != -1)
                    dup2(pipes[ops[op_i].read_end][READ_END], STDIN_FILENO);

                // Let's close all inherited parent's pipes
                int i;

                i = 0;
                while (i < pipe_cnt)
                {
                    if (close(pipes[i][READ_END]) == -1) { perror("close()"); }
                    if (close(pipes[i][WRITE_END]) == -1) { perror("close()"); }
                    ++i;
                }

                // Replace the executable image of this process
                execve(op_argv[0], &op_argv[0], envp);

                fprintf(stderr, "Opps, %s failed\n", op_argv[0]);
                exit(EXIT_FAILURE);

            } // else if (progs[op_i] == 0)
            --op_i;

        } // while (op_i >= 0)

        // Wait for all children to finish
        while (wait(NULL) > 0)
        {
            /*fprintf(stdout, "Parent: Children have finished "
                "the execution, parent is done\n");*/
        }

        // Close all pipes of this prompt
        i = 0;
        while (i < pipe_cnt)
        {
            if (close(pipes[i][READ_END]) == -1) { perror("close()"); }
            if (close(pipes[i][WRITE_END]) == -1) { perror("close()"); }
            ++i;
        }

		free(rline_buf);
		rline_buf = NULL;

	} // while (1) // readline loop
	return 0;
}
