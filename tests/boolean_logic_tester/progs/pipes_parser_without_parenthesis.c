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
		int			pipes[MAX_PIPES_NUM];

		size_t		op_cnt;	// Operand counter
		t_operand	ops[MAX_OPS_NUM]; // operands (programs to launch)

		prompt = rline_buf;
		prompt_len = strlen(prompt);
		op_cnt = 0;
		pipe_cnt = 0;
		while (pi < prompt_len)
		{
			while (prompt[pi] == ' ' && pi < prompt_len)
				++pi;
			if (pi == prompt_len)
				break;
			if ((prompt[pi] >= 'a' && prompt[pi] <= 'z')
			 || (prompt[pi] >= 'A' && prompt[pi] <= 'Z'))
			{
				ops[op_cnt].name[0] = prompt[i];
				ops[op_cnt].name[1] = '\0';
					++pi;
					while (prompt[pi] == ' ' && pi < prompt_len)
						++pi;
					if (pi == prompt_len) // Pipe is at the end of the prompt
					{
						fprintf(stderr, "Parsing error. What is '%c' ?\n", prompt[i]);
						break;
					}
					if (prompt[pi] == '|') // If further goes pipe
					{
						// Let's create a pipe
						if (pipe(pipes[pipe_cnt]) == -1)
						{
							fprintf(stderr, "Can't create pipe\n");
							break;
						}

						if (op_cnt == 0) // If it's the first operand found
						{
							ops[op_cnt]
						}
						else // It's not the first operand
						{

						}
					}
					else
					{
						fprintf(stderr, "Parsing error. What is '%c' ?\n", prompt[i]);
						break;
					}
			}
			else
			{
				fprintf(stderr, "Parsing error. What is '%c' ?\n", prompt[i]);
				break;
			}
			++pi;
		}

		free(rline_buf);
		rline_buf = NULL;
	}
	return 0;
}
