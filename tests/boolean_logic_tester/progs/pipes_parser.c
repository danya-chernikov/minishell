/* This program implements parsing of a basic user prompt
 * containing pipes without parentheses. For simplicity,
 * it launches programs located in the current directory.
 * Each program is named with a single letter of the English
 * alphabet. Lowercase-letter programs always return 0 (success),
 * while uppercase-letter programs always return 1 (failure) */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <stdbool.h>

# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

# include <readline/readline.h>
# include <readline/history.h>

# define EXIT_CMD			"exit"

# define MAX_FORMAT_STR_LEN	64
# define PROMPT_INV_LEN		64	// Maximum length of user's prompt invitation string
# define MAX_PIPES_NUM		128
# define MAX_OPS_NUM		128
# define MAX_PAR_NUM		128	// Maximum parentheses number
# define MAX_TOKENS_NUM		128 // Maximum number of tokens
# define MAX_SUBSHS_NUM		128 // Maximum number of subshells
# define MAX_TOKEN_LEN		128 // Maximum length of each token


# define READ_END			0
# define WRITE_END			1
# define DEFAULT_FD			-1
# define NONE_INDEX			-1
# define NONE_PIPE			-1
# define NONE_PAR_IND		-1
# define NOT_CLOSED_PAR		0	// This parenthesis wasn't closed yet (We didn't pass it)
# define CLOSED_PAR			1	// Thie parenthesis was already closed

# define TOKEN_PIPE			"|"
# define TOKEN_OPEN_PAR		"("
# define TOKEN_CLOSE_PAR	")"
# define TOKEN_AND			"&&"
# define TOKEN_OR			"||"

/* STDIN_FILENO always must be bonded with read-end;
 * STDOUT_FILENO always must be bonded with write-end */
typedef struct s_operand
{
	char	name[2];	// Path to program
	int		read_end;	// stdin
	int		write_end;	// stdout
	pid_t	pid;
}	t_operand;

typedef enum e_token_type
{
    OPERAND,
    PIPE,
    OPEN_PAR,
    CLOSE_PAR,
	AND,
	OR,
	NONE // No tokens were found yet
}   t_token_type;

/* If this token's type is OPERAND
 * we store the pointer to the
 * corresponding operand */
typedef struct s_token
{
	t_token_type	type;
	t_operand		*op;
}	t_token;

/* The value of -1 means the
 * index was not assigned */
typedef struct s_pair
{
	int	first;
	int	second;
}	t_pair;

typedef struct s_engine_data
{
	char		*prompt;
	size_t		pi;							// Prompt index
	size_t		pipe_cnt;
	int			pipes[MAX_PIPES_NUM][2];
	size_t		op_cnt;						// Operand counter
	t_operand	ops[MAX_OPS_NUM];			// Operands (programs to launch)
	int			opar_cnt;					// Opening-parentheses counter (must be int)
	t_pair		open_par[MAX_PAR_NUM];		// Opening-parentheses indexes found
	int			cpar_cnt;					// Closing-parentheses counter
	size_t		close_par[MAX_PAR_NUM][2];	// Closing-rarentheses indexes found and their flags
	size_t		token_cnt;					// Token counter
	t_token		tokens[MAX_TOKENS_NUM];		// Here we store all tokens we found during parsing
}	t_engine_data;

/* Initialization */
int				parser_init(t_engine_data *d, char *rline_buf);
void			remove_right_spaces(char *prompt);
void			init_ops(t_operand *ops);
void			init_close_par(char *prompt, size_t (*par)[2], int *cpar_cnt);
void			init_tokens(t_token *tokens);
void			init_pars(t_pair *pars);
bool			check_empty_par(char *prompt);

/* Parser engine */
bool			parser_engine(t_engine_data *d);
void			handle_open_par(t_engine_data *d, int opar_ind, bool *f_noerr);
int				later_goes_open_par(char *str, size_t ind);
void			skip_spaces(char *prompt, size_t *pi);

/* Execution flow */
int				exec_ops(t_engine_data *d);
int				close_pipes(t_engine_data *d);

/* Debugging */
void			print_parsed_data(t_engine_data *d);
void			print_tokens(t_engine_data *d);
void			print_parentheses(t_engine_data *d);

int	main(void)
{
	t_engine_data	eng_data;
	char			prompt[PROMPT_INV_LEN];
	char			*rline_buf;

	rline_buf = NULL;
	strncpy(prompt, "dchernik@c3r3s6: ", PROMPT_INV_LEN);
	while (1) // readline loop
	{
		rline_buf = readline(prompt);
		/*printf("rline_buf = %p\n", rline_buf);
		printf("rline_buf = \"%s\"\n", rline_buf);*/
		if (strlen(rline_buf) == 0)
		{
			//printf("rline_buf is NULL\n");
			free(rline_buf);
			rline_buf = NULL;
			continue;
		}
		add_history(rline_buf);
		if (!strncmp(rline_buf, EXIT_CMD, strlen(EXIT_CMD)) &&
			strlen(rline_buf) == strlen(EXIT_CMD))
		{
			free(rline_buf);
			rline_buf = NULL;
			break;
		}

		// Let's analyze received prompt/request
		
		if (!parser_init(&eng_data, rline_buf))
			continue;

		if (!parser_engine(&eng_data)) // If we got non-critical parser error
			continue; // Just prompt user to enter another command(s)

		print_parsed_data(&eng_data);
		print_tokens(&eng_data);
		print_parentheses(&eng_data);

		/*if (!exec_ops(&eng_data))
			exit(EXIT_FAILURE);*/

        // Close all pipes of this prompt
		if (!close_pipes(&eng_data))
			exit(EXIT_FAILURE);
			
		free(rline_buf);
		rline_buf = NULL;

	} // while (1) // readline loop
	return 0;
}

int	parser_init(t_engine_data *d, char *rline_buf)
{
	d->pi			= 0;
	d->op_cnt		= 0;
	d->pipe_cnt		= 0;
	d->opar_cnt		= 0;
	d->cpar_cnt		= 0;
	d->token_cnt	= 1; // The first token is always NONE
	d->prompt		= rline_buf;

	remove_right_spaces(d->prompt);	
	init_ops(d->ops); // Initialize operators array
	init_close_par(d->prompt, d->close_par, &d->cpar_cnt);
	init_tokens(d->tokens);
	init_pars(d->open_par);

	if (!check_empty_par(d->prompt))
	{
		fprintf(stderr, "Parsing error: "
			"Empty parentheses are not permitted\n");
		return 0;
	}
	return 1;
}

void	remove_right_spaces(char *prompt)
{
	int	i;

	i = strlen(prompt) - 1;
	if (prompt[i] == ' ')
	{
		while (i >= 0 && prompt[i] == ' ')
		{
			prompt[i] = '\0';
			--i;
		}
	}
}

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

void	init_tokens(t_token *tokens)
{
	size_t	i;

	i = 0;
	while (i < MAX_TOKENS_NUM)
	{
		tokens[i].op = NULL;
		++i; 
	}
	tokens[0].type = NONE;
}

void	init_pars(t_pair *pars)
{
	size_t	i;

	i = 0;
	while (i < MAX_PAR_NUM)
	{
		pars->first = NONE_PAR_IND;
		pars->second = NONE_PAR_IND;
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

/* Parses the user's prompt string by connecting all
 * operands with pipes and launching or exiting subshells
 * when encountering '(' or ')' parentheses, respectively */
bool parser_engine(t_engine_data *d)
{
	size_t			prompt_len;
	bool			f_noerr;	// Parsing error flag
	int				opar_ind;	// Prompt index of the open-parenthesis that goes after pipe
	
	f_noerr = true; // Let's assume there are no errors at first
	prompt_len = strlen(d->prompt);
	while (d->pi < prompt_len) // Going through the entered prompt string
	{
		if (d->prompt[d->pi] == ' ')
			skip_spaces(d->prompt, &d->pi); // Skip possible spaces
		
		// If reached the end of the prompt
		if (d->pi == prompt_len)
			break ;

		// If it's letter
		if (isalpha(d->prompt[d->pi]))
		{
			// Letter-operand can go only after
			// pipe or be the first token or
			// go after '('
			if (d->tokens[d->token_cnt - 1].type != NONE &&
				d->tokens[d->token_cnt - 1].type != PIPE &&
				d->tokens[d->token_cnt - 1].type != OPEN_PAR)
			{
				// Situations like:
				// "a | (b | c) d"
				// "a | b c"
				// "a (b | c)"	
				f_noerr = false;
				fprintf(stderr, "Parsing error: "
					"Before operand must go pipe\n");
				break ;
			}

			// Add this letter in the operators array
			d->ops[d->op_cnt].name[0] = d->prompt[d->pi];
			d->ops[d->op_cnt].name[1] = '\0';

			// Add this operand into the tokens array
			d->tokens[d->token_cnt].type = OPERAND;
			d->tokens[d->token_cnt].op = (t_operand *)&d->ops[d->op_cnt];
			++d->token_cnt;

			++d->op_cnt;

			++d->pi; // Move one symbol forward in prompt

			// Let's see what goes next
			skip_spaces(d->prompt, &d->pi);

			// It means nothing is on the right (just spaces)
			if (d->pi == prompt_len) // We reached the end of the prompt
			{
				if (d->pipe_cnt > 0) // If it's the last operand in the prompt
				{
					d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;
					d->ops[d->op_cnt - 1].write_end = NONE_PIPE;
				}
				// Otherwise, it means our prompt contains only one letter-operand
				break ;
			}
			// Let's see what goes after the letter
			// After the letter goes a pipe
			if (d->prompt[d->pi] == '|') // If further goes pipe
			{
				// A pipe can go only after an operand or after a ')'
				if (d->tokens[d->token_cnt - 1].type != OPERAND &&
					d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
				{
					// Situations like:
					// "a ( | b"
					// "a | | b"
					f_noerr = false;
					fprintf(stderr, "Parsing error: "
						"A pipe can go only after ')' or an operand\n");
					break ;
				}

				// Add this operand into the tokens array
				d->tokens[d->token_cnt].type = PIPE;
				++d->token_cnt;

				// Let's create a pipe
				if (pipe(&d->pipes[d->pipe_cnt][0]) == -1)
				{
					fprintf(stderr, "Can't create pipe: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				if (d->pipe_cnt == 0) // If it's the first operand found
					d->ops[d->op_cnt - 1].read_end = NONE_PIPE;
				else // It's not the first operand
				// Assign to its stdin the previous pipe index
					d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;

				// Assign to its stdout the current pipe index
				d->ops[d->op_cnt - 1].write_end = d->pipe_cnt;

				++d->pipe_cnt;	// Increment pipe index

				opar_ind = later_goes_open_par(d->prompt, d->pi);
				// If after pipe goes opening-parenthesis '('
				if (opar_ind != -1)
				{
					handle_open_par(d, opar_ind, &f_noerr);
					if (!f_noerr)
						break ;
					else
					{
						// If we are here it means ')' was found
						if (d->tokens[d->token_cnt - 1].type == CLOSE_PAR &&
							d->pi == prompt_len)
						{
							if (d->pipe_cnt > 0)
								d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;
						}
						continue ; // Go further by prompt
					}
				}

			} // d->prompt[pi] == '|' // After the letter goes pipe
			
			else // if (prompt[pi] != '|') // After letter goes not pipe
			{
				// If after the letter goes closing-parenthesis ')'
				if (d->prompt[d->pi] == ')')
				{
					// A ')' can go only after an operand or after another ')'
					if (d->tokens[d->token_cnt - 1].type != OPERAND &&
						d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
					{
						// Situations like:
						// "a (b | )"
						// "a | ()"
						f_noerr = false;
						fprintf(stderr, "Parsing error: "
							"A ')' can go only after an operand or another ')'\n");
						break ;
					}

					// If the array of opening-parenthesis is empty
					if (d->opar_cnt == 0)
					{
						f_noerr = false;
						fprintf(stderr, "Parsing error: "
							"Some ')' were found but there are no any '(' to match them\n");
						break ;
					}
					else
					{
						// Add this operand into the tokens array
						d->tokens[d->token_cnt].type = CLOSE_PAR;
						++d->token_cnt;
						break ;
					}
				}
				else // If after the letter goes neither '|' nor ')'
				{
					f_noerr = false;
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
					break ;
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
				// A '(' can go only after a pipe or another '('
				// or also be the first token found
				if (d->tokens[d->token_cnt - 1].type != NONE &&
					d->tokens[d->token_cnt - 1].type != PIPE &&
					d->tokens[d->token_cnt - 1].type != OPEN_PAR) // ~(A + B) = ~A * ~B
				{
					// Situations like:
					// "a | (b | c)(d | e)"
					// "a | b | c ( | a)"
					f_noerr = false;
					fprintf(stderr, "Parsing error: "
						"A '(' can go only after a pipe or another '('\n");
					break ;
				}

				handle_open_par(d, d->pi, &f_noerr);
				if (!f_noerr)
					break ;
				else
				{
					// If we are here it means ')' was found
					if (d->tokens[d->token_cnt - 1].type == CLOSE_PAR &&
						d->pi == prompt_len)
					{
						if (d->pipe_cnt > 0)
							d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;
					}
					continue ; // Go further by prompt
				}
			}
			else if (d->prompt[d->pi] == ')') // If it's closing-parenthesis
			{
				// A ')' can go only after an operand or another ')'
				if (d->tokens[d->token_cnt - 1].type != OPERAND &&
					d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
				{
					// Situations like:
					// "a | (b | c | )"
					// "a | b | ( | a)"
					f_noerr = false;
					fprintf(stderr, "Parsing error: "
						"A ')' can go only after an operand or another ')'\n");
					break ;
				}

				// For example: a | (b | (c))
				if (d->opar_cnt == 0) // If the array of opening-parentheses is empty
				{
					f_noerr = false;
					fprintf(stderr, "Parsing error: "
						"Some ')' were found but there are no any '(' to match them\n");
					break ;
				}
				else
				{
					// Add this operand into the tokens array
					d->tokens[d->token_cnt].type = CLOSE_PAR;
					++d->token_cnt;
					break ;
				}
			}
			else if (d->prompt[d->pi] == '|') // If it's pipe 
			{
				// Pipe can go only after a ')' or after an operand
				if (d->tokens[d->token_cnt - 1].type != CLOSE_PAR &&
					d->tokens[d->token_cnt - 1].type != OPERAND)
				{
					// Situations like:
					// "a | (b | c)( | d"
					// "a | b | | c"
					f_noerr = false;
					fprintf(stderr, "Parsing error: "
						"Pipe can go only after a ')' or an operand\n");
					break;
				}

				// Add this operand into the tokens array
				d->tokens[d->token_cnt].type = PIPE;
				++d->token_cnt;

				// Let's create a pipe
				if (pipe(&d->pipes[d->pipe_cnt][0]) == -1)
				{
					fprintf(stderr, "Can't create pipe: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				// Assign to its stdin the previous pipe index
				d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;

				// Assign to its stdout the current pipe index
				d->ops[d->op_cnt - 1].write_end = d->pipe_cnt;

				++d->pipe_cnt; // Increment pipe index
			
				// Go further by prompt

			} // else if (d->prompt[d->pi] == '|') // If it's pipe 
			else
			{
				f_noerr = false;
				fprintf(stderr, "Parsing error. What is '%c' ?\n", d->prompt[d->pi]);
				break;
			}
		} // else // If it's not a letter
	
		++d->pi;
	} // while (pi < prompt_len)

	return f_noerr;
} // parser_engine() function

/* Handles opening-parenthesis */
void	handle_open_par(t_engine_data *d, int opar_ind, bool *f_noerr)
{
	size_t	last_opar_ind;
	size_t	prompt_len;
	size_t	i;

	// Add this operand into the tokens array
	d->tokens[d->token_cnt].type = OPEN_PAR;
	++d->token_cnt;

	prompt_len = strlen(d->prompt);
	// Add its prompt index to the opening-parentheses array
	d->open_par[d->opar_cnt].first = opar_ind;

	// Move to the next symbol in the prompt after '('
	d->pi = opar_ind + 1;

	++d->opar_cnt; // Increment opening-parenthesis counter

	// When prompt like this "a | b | (" for example
	if (d->pi == prompt_len)
	{
		*f_noerr = false;
		fprintf(stderr, "Parsing error: "
			"prompt terminates with '('\n"); // Reword this
		return; // Go further by prompt
	}

	if (!parser_engine(d))
	{
		*f_noerr = false;
		return;
	}
	/* If we're here the child process was terminated (most likely when
	 * it encountered a closing-parenthesis). Now we have to omit all
	 * symbols between this ')' and the last found '('. Furthermore,
	 * we must remove the index of the last found '(' from `d->opar`
	 * snd decrement `d->op_cnt` */
	
    // Let's say we're here!    
    //printf("parser_engine() completed\n");

	// If the closing-parentheses array is empty
	if (d->cpar_cnt == 0)
	{
		*f_noerr = false;
		fprintf(stderr, "Parsing error: "
			"Some '(' were found but there are no any ')' to match them\n");
		return; // Go further by prompt
	}

	// Let's find, in the closing-parentheses array, the nearest
	// ')' that is not marked as closed to the last found '(' and
	// that is located on the right from '('
	last_opar_ind = d->open_par[d->opar_cnt - 1].first;
	i = 0;
	// The closing-parentheses array is already sorted
	while (i < d->cpar_cnt)
	{
		if (d->close_par[i][0] > last_opar_ind &&
			d->close_par[i][1] == NOT_CLOSED_PAR)
			break;
		++i;
	}

	if (i == d->cpar_cnt) // We went out of the array border
	{
		*f_noerr = false;
		fprintf(stderr, "Parsing error: "
			"Some '(' were found but there are no any ')' to match them\n");
		return; // Go further by prompt
	}

	// Store the pair closing parenthesis index
	// for the last opening parenthesis found
	d->open_par[d->opar_cnt - 1].second = d->close_par[i][0];

	// Move the prompt index to the next symbol in the
	// prompt after the nearest ')' to the last '(' found
	d->pi = d->close_par[i][0] + 1;
	
	// Mark this closing-parenthesis as closed
	d->close_par[i][1] = CLOSED_PAR;

	// By decrementing the opening-parentheses counter we remove
	// the last element from the array of all opening-parentheses
	--d->opar_cnt;
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

// Now we have to launch all operand-programs
int	exec_ops(t_engine_data *d)
{
	t_operand	*op;					// Pointer to the current operand
	pid_t		progs[MAX_OPS_NUM];		// Program PIDs
	pid_t		subshs[MAX_SUBSHS_NUM];	// Our subshells' PIDs
	int			ti;						// Token index (must be int)
	size_t		sh_i;					// Subshell index
	size_t		pi;						// Program index

	// Traversing from right to left the tokens array
	pi = 0;
	sh_i = 0;
	ti = (int)(d->token_cnt) - 1;
	while (ti >= 0)
	{
		if (d->tokens[ti].type == OPERAND)
		{
			progs[pi] = fork();
			if (progs[pi] == -1)
			{
				fprintf(stderr, "Can't fork: %s\n", strerror(errno));
				return (0);
			}
			else if (progs[pi] == 0) // 0 always is returned in the child
			{
				++pi;

				// We're in the new process
				char    *op_argv[2] = { 0, 0 };
				char    *envp[] = { "HOME=/home/user",
									"PATH=/bin:/usr/bin",
									"USER=user", 0 };

				op = d->tokens[ti].op;
				op_argv[0] = &op->name[0];

				// Let's attach pipes to each process (operand)
				if (op->write_end != -1)
					dup2(d->pipes[op->write_end][WRITE_END], STDOUT_FILENO);

				if (op->read_end != -1)
					dup2(d->pipes[op->read_end][READ_END], STDIN_FILENO);

				// Let's close all inherited parent's pipes

				if (!close_pipes(d))
					return (0);

				// Replace the executable image of this process
				execve(op_argv[0], &op_argv[0], envp);

				fprintf(stderr, "Opps, %s failed\n", op_argv[0]);
				return (0);

			} // else if (progs[op_i] == 0)
		}
		else if (d->tokens[ti].type == PIPE)
		{
			// Do nothing
		}
		else if (d->tokens[ti].type == CLOSE_PAR)
		{
			// Let's launch a subshell
			subshs[sh_i] = fork();
			if (subshs[sh_i] == -1)
			{
				fprintf(stderr, "Can't fork: %s\n", strerror(errno));
				return (0);
			}
			if (subshs[sh_i] == 0)
			{
				++sh_i;
				--ti;
				continue ;
			}
		}
		else if (d->tokens[ti].type == OPEN_PAR)
		{
			// Exit the current subshell	
			while (wait(NULL) > 0) {}
			exit(EXIT_SUCCESS);
		}

		--ti;

	} // while (ti >= 0)

	// Wait for all children to finish
	//printf("Waiting for children...\n");
	while (wait(NULL) > 0)
	{
		/*fprintf(stdout, "Parent: Children have finished "
			"the execution, parent is done\n");*/
	}
	return (1);
}

int	close_pipes(t_engine_data *d)
{
	size_t	i;

	i = 0;
	while (i < d->pipe_cnt)
	{
		if (close(d->pipes[i][READ_END]) == -1)
		{
			perror("close()");
			return 0;
		}
		if (close(d->pipes[i][WRITE_END]) == -1)
		{
			perror("close()");
			return 0;
		}
		++i;
	}
	return 1;
}

void	print_parsed_data(t_engine_data *d)
{
	size_t	i;

	// Let's output the pipes we found
	i = 0; 
	printf("\nPipes:\n");
	while (i < d->pipe_cnt)
	{
		printf("%lu: [%d] [%d]\n", i + 1,
			d->pipes[i][READ_END], d->pipes[i][WRITE_END]);
		++i;
	}
	printf("\n");

	// Let's output the operands we found
	i = 0; 
	printf("\nOperands:\n");
	while (i < d->op_cnt)
	{
		printf("%lu: [%s] [%d] [%d]\n", i + 1,
			d->ops[i].name, d->ops[i].read_end, d->ops[i].write_end);
		++i;
	}
	printf("\n");
}

void	print_tokens(t_engine_data *d)
{
	char	format[MAX_FORMAT_STR_LEN];
	size_t	i;

	strncpy(format, "%d. %s\n", MAX_FORMAT_STR_LEN);
	i = 0;
	printf("\nTokens:\n");
	while (i < d->token_cnt)
	{
		if (d->tokens[i].type == OPERAND)
			printf(format, i + 1, d->tokens[i].op->name);
		else if (d->tokens[i].type == PIPE)
			printf(format, i + 1, TOKEN_PIPE);
		else if (d->tokens[i].type == OPEN_PAR)
			printf(format, i + 1, TOKEN_OPEN_PAR);
		else if (d->tokens[i].type == CLOSE_PAR)
			printf(format, i + 1, TOKEN_CLOSE_PAR);
		else if (d->tokens[i].type == AND)
			printf(format, i + 1, TOKEN_AND);
		else if (d->tokens[i].type == OR)
			printf(format, i + 1, TOKEN_OR);
		++i;
	}
	printf("\n");
}

/* If there were no errors at the parsing stage,
 * the number of opening and closing parentheses
 * would be equal */
void	print_parentheses(t_engine_data *d)
{	
	size_t	i;

	i = 0;
	printf("\nParentheses:\n");
	printf("#\t(\t)\n");
	while (i < d->cpar_cnt)
	{
		printf("%lu\t%d\t%d\n",
			i + 1,
			d->open_par[i].first,
			d->open_par[i].second);
		++i;
	}
	printf("\n");
}
