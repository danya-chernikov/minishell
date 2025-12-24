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

typedef struct s_engine_data
{
	char		*prompt;
	size_t		pi; // Prompt index
	size_t		pipe_cnt;
	int			pipes[MAX_PIPES_NUM][2];
	size_t		op_cnt;	// Operand counter
	t_operand	ops[MAX_OPS_NUM]; // Operands (programs to launch)
	size_t		p_cnt; // Parentheses counter
	
}	t_engine_data;

void	init_ops(t_operand *ops);

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
		t_engine_data	eng_data;

		eng_data->pi		= 0;
		eng_data->op_cnt	= 0;
		eng_data->pipe_cnt	= 0;
		eng_data->p_cnt		= 0;
		eng_data->prompt	= rline_buf;

		init_ops(eng_data->ops);

		parser_engine(&eng_data);

        // Close all pipes of this prompt
        i = 0;
        while (i < d->pipe_cnt)
        {
            if (close(d->pipes[i][READ_END]) == -1) { perror("close()"); }
            if (close(d->pipes[i][WRITE_END]) == -1) { perror("close()"); }
            ++i;
        }

		free(rline_buf);
		rline_buf = NULL;

	} // while (1) // readline loop
	return 0;
}

int	parser_engine(t_engine_data *d)
{
	size_t		i; // First auxiliary index	
	size_t		prompt_len;
	bool        f_err;
	
	f_err = false;
	prompt_len = strlen(d->prompt);
	while (d->pi < prompt_len)
	{
		// Skip possible spaces
		while (d->prompt[pi] == ' ' && d->pi < prompt_len)
			++(d->pi);
		// If reached the end of prompt
		if ((d->pi) == prompt_len)
			break;
		// If it's letter
		if (isalpha(d->prompt[d->pi]))
		{
			d->ops[d->op_cnt].name[0] = d->prompt[d->pi];
			d->ops[d->op_cnt].name[1] = '\0';
			++(d->pi);
			// Let's see what's goes next
			while (d->prompt[d->pi] == ' ' && d->pi < prompt_len)
				++(d->pi);
			// It means nothing is on the right (just spaces)
			if (d->pi == prompt_len)
			{
				if (d->pipe_cnt > 0)
				{
					d->ops[d->op_cnt].read_end = d->pipe_cnt;
					d->ops[d->op_cnt].write_end = -1;
				}
				++(d->op_cnt);
				break;
			}

			// After letter goes pipe
			if (d->prompt[d->pi] == '|') // If further goes pipe
			{
				// Let's create a pipe
				if (pipe(d->pipes[d->pipe_cnt][0]) == -1)
				{
					fprintf(stderr, "Can't create pipe: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				if (d->pipe_cnt == 0) // If it's the first operand found
					d->ops[d->op_cnt].read_end = -1;
				else // It's not the first operand
					d->ops[d->op_cnt].read_end = d->pipe_cnt - 1;

				d->ops[d->op_cnt].write_end = d->pipe_cnt;

				// If after pipe goes (
				if (later_goes_open_par(d->prompt, d->pi))
				{
					++(d->p_cnt);

					// Run a subshell
					pid_t subsh_pid = fork();
					if (subsh_pid == -1)
					{
						fprintf(stderr, "Can't fork: %s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}
					else if (subsh_pid == 0)
					{
						// We're in our copy, in another process

					}
				}

				++(d->pipe_cnt);
			}
			else // if (prompt[pi] != '|')
			{
				f_err = true;
				if (isalpha(d->prompt[d->pi]))
				{
					fprintf(stderr, "Parsing error: "
						"After operand cannot go another operand\n");
				}
				else
				{
					fprintf(stderr, "Parsing error. What is '%c' ?\n",
						d->prompt[d->pi]);
				}
				break;
			}

			++(d->op_cnt);

		} // if ((prompt[pi] >= 'a' && prompt[pi] <= 'z')

		else // If it's not a letter
		{
			// In case the first symbol going after omitted spaces is '('
			if (d->prompt[d->pi] == '(') // For example: (a | b) | c
			{
				++(d->p_cnt);

				// Run a subshell
				pid_t subsh_pid = fork();
				if (subsh_pid == -1)
				{
					fprintf(stderr, "Can't fork: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				else if (subsh_pid == 0)
				{

				}
			}
			else // It's not '('
			{
			
				f_err = true;
				fprintf(stderr, "Parsing error. What is '%c' ?\n", d->prompt[d->pi]);
				break;
			}
		}
		++(d->pi);
	} // while (pi < prompt_len)

	if (f_err) // In case of non-critical parsing error
		continue;

	// Let's output the pipes we found
	i = 0; 
	while (i < d->pipe_cnt)
	{
		printf("%lu: [%d] [%d]\n", i + 1,
			d->pipes[i][READ_END], d->pipes[i][WRITE_END]);
		++i;
	}
	printf("\n");

	// Let's output the operators we found
	i = 0; 
	while (i < d->op_cnt)
	{
		printf("%lu: [%s] [%d] [%d]\n", i + 1,
			d->ops[i].name, d->ops[i].read_end, d->ops[i].write_end);
		++i;
	}
	printf("\n");

	// Now we have to launch all operand-programs
	pid_t   progs[MAX_OPS_NUM]; // Program PIDs
	int     op_i; // Operand index

	// Go from right to left operand
	op_i = (int)(d->op_cnt) - 1;
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

			op_argv[0] = d->ops[op_i].name[0]; // I had & here before

			// Let's attach pipes to each process (operand)
			if (d->ops[op_i].write_end != -1)
				dup2(d->pipes[ d->ops[op_i].write_end ][WRITE_END], STDOUT_FILENO);

			if (d->ops[op_i].read_end != -1)
				dup2(d->pipes[ d->ops[op_i].read_end ][READ_END], STDIN_FILENO);

			// Let's close all inherited parent's pipes
			int i; // Do we really need to declare it here?

			i = 0;
			while (i < d->pipe_cnt)
			{
				if (close(d->pipes[i][READ_END]) == -1) { perror("close()"); }
				if (close(d->pipes[i][WRITE_END]) == -1) { perror("close()"); }
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

	return 0;
} // parser_engine() function

/* Assign the default value to
 * the pipes of all operators */
void	init_ops(t_operand *ops)
{
	size_t	i;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		ops[i]->write_end = DEFAULT_FD;
		ops[i]->read_end = DEFAULT_FD;
		++i;
	}
}

/* Checks whether there is an opening parenthesis
 * later to the right, starting from index ind,
 * ignoring only spaces. If it finds '(' while
 * moving to the right and skipping all spaces,
 * it returns 1. Otherwise, it returns 0. If it
 * encounters any symbol other than a space before
 * finding '(', it also returns 0 */
int	later_goes_open_par(char *str, size_t ind)
{
	size_t	slen;

	++ind;
	slen = strlen(str);
	while (ind < slen)
	{
		if (str[ind] == '(')
			return 1;
		if (str[ind] == ' ')
			++ind;
		else
			return 0;
	}
	return 0;
}
