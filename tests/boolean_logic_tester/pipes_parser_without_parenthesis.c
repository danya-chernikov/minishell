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

	while (1)
	{
		rline_buf = readline(prompt);
		printf("rline_buf = %p\n", rline_buf);
		printf("rline_buf = \"%s\"\n", rline_buf);
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
		size_t		j; // Second auxiliary index
	
		size_t		pi; // Prompt index
		size_t		prompt_len;
		char		*prompt;

		size_t		pipe_cnt;
		int			pipes[MAX_PIPES_NUM][2];

		size_t		op_cnt;	// Operand counter
		t_operand	ops[MAX_OPS_NUM]; // operands (programs to launch)

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
                    printf("Nothing is on the right\n");
                    if (pipe_cnt > 0)
                    {
                        ops[op_cnt].read_end = pipes[pipe_cnt][0];
                        ops[op_cnt].write_end = 0;
                    }
                    ++op_cnt;
                    break;
                }

                if (prompt[pi] == '|') // If further goes pipe
                {
                    // Let's create a pipe
                    if (pipe(&pipes[pipe_cnt][0]) == -1)
                    {
                        fprintf(stderr, "Can't create pipe\n");
                        break;
                    }

                    if (pipe_cnt == 0) // If it's the first operand found
                        ops[op_cnt].read_end = 0;
                    else // It's not the first operand
                        ops[op_cnt].read_end = pipes[pipe_cnt - 1][0];
                    ops[op_cnt].write_end = pipes[pipe_cnt][1];
                    ++pipe_cnt;
                }
                else // if (prompt[pi] != '|')
                {
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
				fprintf(stderr, "Parsing error. What is '%c' ?\n", prompt[pi]);
				break;
            }
			++pi;
		} // while (pi < prompt_len)

        // Let's output the pipes we found
        i = 0; 
        while (i < pipe_cnt)
        {
            printf("%lu: [%d] [%d]\n", i + 1, pipes[i][0], pipes[i][1]);
            ++i;
        }
        printf("\n");

        // Let's output the operators we found
        i = 0; 
        while (i < op_cnt)
        {
            printf("%lu: %s\n", i + 1, ops[i].name);
            ++i;
        }
        printf("\n");

		free(rline_buf);
		rline_buf = NULL;
	}
	return 0;
}
