/* This program implements parsing of a basic user prompt
 * containing pipes without parentheses. For simplicity,
 * it launches programs located in the current directory.
 * Each program is named with a single letter of the English
 * alphabet. Lowercase-letter programs always return 0 (success),
 * while uppercase-letter programs always return 1 (failure) */

/*
 * The question is: should we consider sequences like
 * () in bash it leads error
 * (()) in bash this is okay
 * (((  ))) error
 * (((( )))) error
 * as erroneous?
 * Answer: Yes! All such sequences we're gonna consider as an error
 * */
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
#define MAX_PAR_NUM		128	// Maximum parentheses number
#define READ_END		0
#define WRITE_END		1
#define DEFAULT_FD      -1
#define NONE_INDEX		-1
#define NONE_PIPE		-1
#define NOT_CLOSED_PAR	0	// This parenthesis wasn't closed yet (We didn't pass it)
#define CLOSED_PAR		1	// Thie parenthesis was already closed

/* STDIN_FILENO always must be bonded with read-end;
 * STDOUT_FILENO always must be bonded with write-end */
typedef struct s_operand
{
	char	name[2];	// path to program
	int		read_end;	// stdin
	int		write_end;	// stdout
}	t_operand;

typedef enum e_token
{
    OPERAND,
    PIPE,
    OPEN_PAR,
    CLOSE_PAR
}   t_token;

typedef struct s_engine_data
{
	char		*prompt;
	size_t		pi;							// Prompt index
	size_t		pipe_cnt;
	int			pipes[MAX_PIPES_NUM][2];
	size_t		op_cnt;						// Operand counter
	t_operand	ops[MAX_OPS_NUM];			// Operands (programs to launch)
	int			opar_cnt;					// Opening-parentheses counter (must be int)
	size_t		open_par[MAX_PAR_NUM];		// Opening-parentheses indexes found
	int			cpar_cnt;					// Closing-parentheses counter
	size_t		close_par[MAX_PAR_NUM][2];	// Closing-rarentheses indexes found and their flags
    t_token     priv_token;
	
}	t_engine_data;

void	init_ops(t_operand *ops);
void	init_close_par(char *prompt, size_t (*par)[2], int *cpar_cnt);
bool	check_empty_par(char *prompt);

int		parser_engine(t_engine_data *d);
int		later_goes_open_par(char *str, size_t ind);
void	skip_spaces(char *prompt, size_t *pi);
void	handle_open_par(t_engine_data *d, int opar_ind, bool *f_err);

int	main(void)
{
	char	prompt[64];
	char	*rline_buf;
	size_t	i;

	rline_buf = NULL;
	strncpy(prompt, "dchernik@c3r3s6: ", 64);
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

		eng_data.pi			= 0;
		eng_data.op_cnt		= 0;
		eng_data.pipe_cnt	= 0;
		eng_data.opar_cnt	= 0;
		eng_data.cpar_cnt	= 0;
		eng_data.prompt	= rline_buf;

		init_ops(eng_data.ops); // Initialize operators array
		init_close_par(eng_data.prompt, eng_data.close_par, &eng_data.cpar_cnt);
		if (!check_empty_par(eng_data.prompt))
		{
			fprintf(stderr, "Parsing error: "
				"Empty parentheses are not permitted\n");
			continue;
		}

		if (!parser_engine(&eng_data)) // If we got non-critical parser error
			continue; // Just prompt user to enter another command(s)

        // Close all pipes of this prompt
        i = 0;
        while (i < eng_data.pipe_cnt)
        {
            if (close(eng_data.pipes[i][READ_END]) == -1) { perror("close()"); }
            if (close(eng_data.pipes[i][WRITE_END]) == -1) { perror("close()"); }
            ++i;
        }

		free(rline_buf);
		rline_buf = NULL;

	} // while (1) // readline loop
	return 0;
}

int	parser_engine(t_engine_data *d)
{
	size_t		i;			// First auxiliary index	
	size_t		prompt_len;
	bool        f_err;		// Parsing error flag
	
	f_err = false;
	prompt_len = strlen(d->prompt);
	while (d->pi < prompt_len) // Going through the entered prompt string
	{
		if (d->prompt[d->pi] == ' ')
			skip_spaces(d->prompt, &d->pi); // Skip possible spaces
		
		// If reached the end of the prompt
		if (d->pi == prompt_len)
			break;

		// If it's letter
		if (isalpha(d->prompt[d->pi]))
		{
			// Add this letter in the operators array
			d->ops[d->op_cnt].name[0] = d->prompt[d->pi];
			d->ops[d->op_cnt].name[1] = '\0';
			++(d->pi);

			// Let's see what goes next
			skip_spaces(d->prompt, &d->pi);

			// It means nothing is on the right (just spaces)
			if (d->pi == prompt_len) // We reached the end of the prompt
			{
				if (d->pipe_cnt > 0) // If it's the last operand in the prompt
				{
					d->ops[d->op_cnt].read_end = d->pipe_cnt;
					d->ops[d->op_cnt].write_end = NONE_PIPE;
				}
				// Otherwise, it means our prompt contains only one letter-operand
				++(d->op_cnt);
				break;
			}
			// Let's see what goes after the letter
			// After the letter goes a pipe
			if (d->prompt[d->pi] == '|') // If further goes pipe
			{
				// Let's create a pipe
				if (pipe(&d->pipes[d->pipe_cnt][0]) == -1)
				{
					fprintf(stderr, "Can't create pipe: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				if (d->pipe_cnt == 0) // If it's the first operand found
					d->ops[d->op_cnt].read_end = NONE_PIPE;
				else // It's not the first operand
				// Assign to its stdin the previous pipe index
					d->ops[d->op_cnt].read_end = d->pipe_cnt - 1;

				// Assign to its stdout the current pipe index
				d->ops[d->op_cnt].write_end = d->pipe_cnt;

				++(d->pipe_cnt);	// Increment pipe index
				++(d->op_cnt);		// Increment operand index

				int	opar_ind = later_goes_open_par(d->prompt, d->pi);
				// If after pipe goes opening-parenthesis '('
				if (opar_ind != -1)
				{
					handle_open_par(d, opar_ind, &f_err);
					if (f_err)
						break;
					else
						continue; // Go further by prompt
				}
				else {} // Nothing

			} // d->prompt[pi] == '|' // After letter goes pipe
			
			else // if (prompt[pi] != '|') // After letter goes not pipe
			{
				// If after the letter goes closing-parenthesis ')'
				if (d->prompt[d->pi] == ')')
				{
					// If the array of opening-parenthesis is empty
					if (d->opar_cnt == 0)
					{
						f_err = true;
						fprintf(stderr, "Parsing error: "
							"Some ')' were found but there are no any '(' to match them\n");
						break;
					}
					else
					{
						// Now if we're gonna exit the current subshell
						exit(EXIT_SUCCESS);
					}
				}
				else // If after the letter goes neither '|' nor ')'
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
			}

		} // if ((prompt[pi] >= 'a' && prompt[pi] <= 'z')

		else // If it's not a letter
		{
			// In case the first symbol going after omitted spaces is '('
			// Or in other words
			// If the user command(s) starts with an opening parenthesis
			if (d->prompt[d->pi] == '(') // For example: (a | b) | c
			{
				handle_open_par(d, d->pi, &f_err);
				if (f_err)
					break;
				else
					continue; // Go further by prompt
			}
			else if (d->prompt[d->pi] == ')') // If it's closing-parenthesis
			{
				// For example: a | (b | (c))
				if (d->opar_cnt == 0) // If the array of opening-parenthesis is empty
				{
					f_err = true;
					fprintf(stderr, "Parsing error: "
						"Some ')' were found but there are no any '(' to match them\n");
					break;
				}
				else
				{
					// Now if we're gonna exit the current subshell,
					exit(EXIT_SUCCESS);
				}
			}
			else if (d->prompt[d->pi] == '|') // If it's pipe 
			{
				// Most probably this pipe goes after closing-parenthesis
				if (d->op_cnt == 0) // If the array of operands is empty
				{
					f_err = true;
					fprintf(stderr, "Parsing error: "
						"A '|' cannot go before any operand\n"); // Reword
					break;
				}
				else
				{
					// Let's create a pipe
					if (pipe(&d->pipes[d->pipe_cnt][0]) == -1)
					{
						fprintf(stderr, "Can't create pipe: %s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}

					// Assign to its stdout the current pipe index
					d->ops[d->op_cnt].write_end = d->pipe_cnt;

					++(d->pipe_cnt); // Increment pipe index
				
					continue; // Go further by prompt
				}
			}
			else
			{
				f_err = true;
				fprintf(stderr, "Parsing error. What is '%c' ?\n", d->prompt[d->pi]);
				break;
			}
		}
		++(d->pi);
	} // while (pi < prompt_len)

	if (f_err) // In case of non-critical parsing error
		return f_err; // Just prompt user to enter another command(s)

	// If after exiting the main loop we ended up here
	// it mean there were not any parsing errors found
	
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
	pid_t   progs[MAX_OPS_NUM];	// Program PIDs
	int     op_i;				// Operand index

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

			op_argv[0] = &d->ops[op_i].name[0]; // I had & here before

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

	return f_err;
} // parser_engine() function

/* Assign the default value to
 * the pipes of all operators */
void	init_ops(t_operand *ops)
{
	size_t	i;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		ops[i].write_end = DEFAULT_FD;
		ops[i].read_end = DEFAULT_FD;
		++i;
	}
}

/* Counts all closing parentheses and remembers their indexes */
void	init_close_par(char *prompt, size_t (*par)[2], int *cpar_cnt)
{
	size_t	i;

	i = 0;
	while (i < strlen(prompt))
	{
		if (prompt[i] == ')')
		{
			par[*cpar_cnt][0] = i;
			par[*cpar_cnt][1] = NOT_CLOSED_PAR;
			++(*cpar_cnt);
		}
		++i;
	}
}

/* Checks for existance of empty parentheses.
 * Sequences like: (), (( )), (((  ))), and etc.
 * Returns true if there are no empty sequences*/
bool	check_empty_par(char *prompt)
{
	size_t	i;

	i = 0;
	while (i < strlen(prompt))
	{
		if (prompt[i] == '(' )
		{
			++i;
			skip_spaces(prompt, &i);
			if (i == strlen(prompt)) // Parsing error
				return false;
			if (prompt[i] == ')')
				return false;
		}
		++i;
	}
	return true;
}

/* Checks whether there is an opening parenthesis
 * later to the right, starting from index ind,
 * ignoring only spaces. If it finds '(' while
 * moving to the right and skipping all spaces,
 * it returns its index. Otherwise, it returns -1.
 * If it encounters any symbol other than a space
 * before finding '(', it also returns -1 */
int	later_goes_open_par(char *str, size_t ind)
{
	size_t	slen;

	++ind;
	slen = strlen(str);
	while (ind < slen)
	{
		if (str[ind] == '(')
			return ind;
		if (str[ind] == ' ')
			++ind;
		else
			return -1;
	}
	return -1;
}

void	skip_spaces(char *prompt, size_t *pi)
{
	while (prompt[*pi] == ' ' && *pi < strlen(prompt))
		++(*pi);
}

/* Handles opening-parenthesis */
void	handle_open_par(t_engine_data *d, int opar_ind, bool *f_err)
{
	size_t	prompt_len;
	size_t	i;

	prompt_len = strlen(d->prompt);
	// Add its prompt index to the opening-parentheses array
	d->open_par[d->opar_cnt] = opar_ind;

	// Move to the next symbol in the prompt after '('
	d->pi = opar_ind + 1;

	++(d->opar_cnt); // Increment opening-parenthesis counter

	// When prompt like this "a | b | (" for example
	if (d->pi == prompt_len)
	{
		*f_err = true;
		fprintf(stderr, "Parsing error: "
			"prompt terminates with '('\n"); // Reword this
		return; // Go further by prompt
	}

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
		parser_engine(d);
	}
	/* If we're here the child process was terminated (most likely when
	 * it encountered a closing-parenthesis). Now we have to omit all
	 * symbols between this ')' and the last found '('. Furthermore,
	 * we must remove the index of the last found '(' from `d->opar`
	 * snd decrement `d->op_cnt` */
	
    // Let's say we're here!    
    printf("Child process was completed\n");
    
	// If the closing-parentheses array is empty
	if (d->cpar_cnt == 0)
	{
		*f_err = true;
		fprintf(stderr, "Parsing error: "
			"Some '(' were found but there are no any ')' to match them\n");
		return; // Go further by prompt
	}

	// Let's find, in the closing-parentheses array, the nearest
	// ')' that is not marked as closed to the last found '(' and
	// that is located on the right from '('
	size_t	last_opar_ind = d->open_par[d->opar_cnt - 1];
	i = 0;
	// The closing-parentheses array is already sorted
	while (i < d->cpar_cnt)
	{
		if (d->close_par[i][0] > last_opar_ind &&
			d->close_par[i][1] == NOT_CLOSED_PAR)
			break;
		++i;
	}

	// Move the prompt index to the next symbol in the
	// prompt after the nearest ')' to the last '(' found
	d->pi = d->close_par[i][0] + 1;
	
	// Mark this closing-parenthesis as closed
	d->close_par[i][1] = CLOSED_PAR;
	
}
