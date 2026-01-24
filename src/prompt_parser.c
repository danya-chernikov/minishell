#include "prompt_parser.h"

/* The first token is always NONE because
 * we always check previous token value */
int	parser_init(t_parser_data *d, char *rline_buf)
{
	d->pi = 0;
	d->op_cnt = 0;
	d->pipe_cnt = 0;
	d->opar_num = 0;
	d->opar_cnt = 0;
	d->cpar_cnt = 0;
	d->par_cnt = 0;
	d->token_cnt = 1;
	d->qpair_cnt = 0;
	d->prompt = rline_buf;

	remove_right_spaces(d->prompt);

	if (ops_init(d->ops) == -1) // Initialize operators array
		return (COMMON_SYS_ERR);
	open_par_init(d);
	close_par_init(d);
	tokens_init(d);
	pars_init(d->pars);

	return (COMMON_SUCCESS);
}

/* Parses the user's prompt string by connecting all
 * operands with pipes and launching or exiting subshells
 * when encountering '(' or ')' parentheses, respectively */
bool	parser_engine(t_parser_data *d)
{
	size_t	prompt_len;
	bool	f_noerr;	// Parsing error flag
	int		opar_ind;	// Prompt index of the open-parenthesis that goes after pipe
	
	f_noerr = true; // Let's assume there are no errors at first
	prompt_len = ft_strlen(d->prompt);

	if (!d->prompt || prompt_len == 0)
		return f_noerr;

	while (d->pi < prompt_len) // Going through the entered prompt string
	{
		if (d->prompt[d->pi] == ' ')
			skip_spaces(d->prompt, &d->pi); // Skip possible spaces
		
		// If reached the end of the prompt
		if (d->pi == prompt_len)
			break ;

		// If it's not special character
		// or a special character but that is located inside a quotes interval
		if (!is_special_char(d->prompt[d->pi]) ||
			(is_special_char(d->prompt[d->pi]) && is_inside_quotes(d, d->pi)))
		{
			// Letter-operand can go only after pipe,
			// '(', &&, || or be the first token
			if (d->tokens[d->token_cnt - 1].type != PIPE && // Previous token is not a pipe
				d->tokens[d->token_cnt - 1].type != OPEN_PAR &&
				d->tokens[d->token_cnt - 1].type != AND &&
				d->tokens[d->token_cnt - 1].type != OR &&
				d->tokens[d->token_cnt - 1].type != NONE) // not the first token
			{
				handle_prompt_parser_error(&f_noerr);
				break ;
			}

			// Traverse all symbols until the next operand or
			// parenthesis, and add them to the operands array.
			// All this symbols will represent a new operand
			if (operand_push(d, prompt_len) != COMMON_SUCCESS)
			{
				f_noerr = false;
				break ;
			}
			token_push(d, OPERAND); // Add this operand into the tokens array
			skip_spaces(d->prompt, &d->pi); // Let's see what goes next

			// Here we should check whether this is the end of the current pipeline

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
			if (its_PIPE(d->prompt, prompt_len, d->pi)) // If further goes pipe
			{
				// A pipe can go only after an operand or after a ')'
				if (d->tokens[d->token_cnt - 1].type != OPERAND &&
					d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
				{
					handle_prompt_parser_error(&f_noerr);
					break ;
				}

				token_push(d, PIPE); // Add this operand into the tokens array

				// Let's create a pipe
				if (pipe(&d->pipes[d->pipe_cnt][0]) == -1)
				{
					print_sys_error("Can't create pipe");
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
				// If it's not a pipe, the other possible tokens
				// that may follow a letter-operand are ), &&, or ||

				// If after the letter goes closing-parenthesis ')'
				if (d->prompt[d->pi] == ')')
				{
					handle_close_par(d, &f_noerr);
					break ;
				}
				// If after the letter goes &&
				else if (its_logical_AND(d->prompt, prompt_len, d->pi))
				{
					// Add this operator into the tokens array
					token_push(d, AND);
					d->pi += 2;
					continue ; // Go further by prompt
				}
				// If after the letter goes ||
				else if (its_logical_OR(d->prompt, prompt_len, d->pi))
				{
					// Add this operator into the tokens array
					token_push(d, OR);
					d->pi += 2;
					continue ; // Go further by prompt
				}
				else // If after the letter goes neither '|' nor ')'
				{
					handle_prompt_parser_error(&f_noerr);
					break ;
				}
			}

		} // if (is_any_symbol(prompt[pi]))

		// If it's a special character outside the parenthesis
		else if (is_special_char(d->prompt[d->pi]) && !is_inside_quotes(d, d->pi))
		{
			// In case the first symbol going after ommitted spaces is '('
			// Or in other words
			// If the user command(s) starts with an opening parenthesis
			if (d->prompt[d->pi] == '(') // For example: (a | b) | c
			{
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
				handle_close_par(d, &f_noerr);
				break ;
			}

			if (its_PIPE(d->prompt, prompt_len, d->pi)) // If it's pipe
			{
				// Pipe can go only after a ')' or after an operand
				if (d->tokens[d->token_cnt - 1].type != CLOSE_PAR &&
					d->tokens[d->token_cnt - 1].type != OPERAND)
				{
					handle_prompt_parser_error(&f_noerr);
					break;
				}

				token_push(d, PIPE); // Add this operand into the tokens array

				// Let's create a pipe
				if (pipe(&d->pipes[d->pipe_cnt][0]) == -1)
				{
					print_sys_error("Can't create pipe");
					exit(EXIT_FAILURE);
				}

				// Assign to its stdin the previous pipe index
				d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;

				// Assign to its stdout the current pipe index
				d->ops[d->op_cnt - 1].write_end = d->pipe_cnt;

				++d->pipe_cnt; // Increment pipe index
			
				// Go further by prompt

			} // else if (d->prompt[d->pi] == '|') // If it's pipe 	
			
			else if (its_logical_AND(d->prompt, prompt_len, d->pi)) // If it's &&
			{
				// && can go after an operand of a ')'
				if (d->tokens[d->token_cnt - 1].type != OPERAND &&
					d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
				{
					handle_prompt_parser_error(&f_noerr);
					break ;
				}

				token_push(d, AND);
				d->pi += 2;
				continue ; // Go further by prompt
			}

			// If after the letter goes ||
			else if (its_logical_OR(d->prompt, prompt_len, d->pi)) // If it's ||
			{
				// || can go after an operand of a ')'
				if (d->tokens[d->token_cnt - 1].type != OPERAND &&
					d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
				{
					handle_prompt_parser_error(&f_noerr);
					break ;
				}

				token_push(d, OR);
				d->pi += 2;
				continue ; // Go further by prompt
			}

			else
			{
				handle_prompt_parser_error(&f_noerr);
				break;
			}
		} // else // If it's not a letter
	
		++d->pi;
	} // while (pi < prompt_len)

	return f_noerr;
} // parser_engine() function

/* Handles opening-parenthesis */
void	handle_open_par(t_parser_data *d, int opar_ind, bool *f_noerr)
{
	size_t	last_opar_ind;
	size_t	prompt_len;
	size_t	i;

	// A '(' can go only after: pipe, another
	// '(', &&, || or be the first token found
	if (d->tokens[d->token_cnt - 1].type != PIPE &&
		d->tokens[d->token_cnt - 1].type != OPEN_PAR &&
		d->tokens[d->token_cnt - 1].type != AND &&
		d->tokens[d->token_cnt - 1].type != OR &&
		d->tokens[d->token_cnt - 1].type != NONE)
	{
		handle_prompt_parser_error(f_noerr);
		return ;
	}

	// Add this operand into the tokens array
	d->tokens[d->token_cnt].type = OPEN_PAR;
	d->tokens[d->token_cnt].start_pi = opar_ind;
	++d->token_cnt;

	prompt_len = ft_strlen(d->prompt);
	// Add its prompt index to the opening-parentheses array
	d->open_par[d->opar_cnt] = opar_ind;

	// Add this opening-parenthesis index to
	// the pair of all all parentheses pairs
	d->pars[d->par_cnt].first = opar_ind;
	++d->par_cnt;

	// Move to the next symbol in the prompt after '('
	d->pi = opar_ind + 1;

	++d->opar_cnt; // Increment opening-parenthesis counter

	// When prompt like this "a | b | (" for example
	if (d->pi == prompt_len)
	{
		handle_prompt_parser_error(f_noerr);
		return ; // Go further by prompt
	}

	if (!parser_engine(d))
	{
		*f_noerr = false;
		return ;
	}
	/* If we're here the child process was
	 * terminated (most likely when it
	 * encountered a closing-parenthesis).
	 * Now we have to omit all symbols between
	 * this ')' and the last found '('.
	 * Furthermore, we must remove the index
	 * of the last found '(' from `d->opar`
	 * and decrement `d->opar_cnt` */	

	// If the closing-parentheses array is empty
	if (d->cpar_cnt == 0)
	{
		handle_prompt_parser_error(f_noerr);
		return; // Go further by prompt
	}

	// Let's find, in the closing-parentheses array, the nearest
	// ')' that is not marked as closed to the last found '(' and
	// that is located on the right from '('
	last_opar_ind = d->open_par[d->opar_cnt - 1];
	i = 0;
	// The closing-parentheses array is already sorted
	while (i < (size_t)d->cpar_cnt)
	{
		if (d->close_par[i][0] > last_opar_ind &&
			d->close_par[i][1] == NOT_CLOSED_PAR)
			break ;
		++i;
	}

	if (i == (size_t)d->cpar_cnt) // We went out of the array border
	{
		handle_prompt_parser_error(f_noerr);
		return ; // Go further by prompt
	}

	// Move the prompt index to the next symbol in the
	// prompt after the nearest ')' to the last '(' found
	d->pi = d->close_par[i][0] + 1;
	
	// Mark this closing-parenthesis as closed
	d->close_par[i][1] = CLOSED_PAR;

	// By decrementing the opening-parentheses counter we remove
	// the last element from the array of all opening-parentheses
	--d->opar_cnt;
}

void	handle_close_par(t_parser_data *d, bool *f_noerr)
{
	size_t	last_cpar_ind;	// Last closing-parenthesis index
	size_t	pair_opar_ind;
	size_t	i;

	// A ')' can go only after an operand or after another ')'
	if (d->tokens[d->token_cnt - 1].type != OPERAND &&
		d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
	{
		handle_prompt_parser_error(f_noerr);
		return ;
	}

	// If the array of opening-parenthesis is empty
	if (d->opar_cnt == 0)
	{
		handle_prompt_parser_error(f_noerr);
		return ;
	}

	// Add this operand into the tokens array
	d->tokens[d->token_cnt].type = CLOSE_PAR;
	d->tokens[d->token_cnt].start_pi = d->pi;
	++d->token_cnt;

	// Let's find the nearest to us (to `d->pi`)
	// not-yet-closed opening parenthesis to the
	// left from `d->pi` in `d->open_par`
	last_cpar_ind = d->pi;
	i = 0;
	pair_opar_ind = i;
	// The `opar_num` after calculating it on the
	// initialization stage will never be changed
	// meanwhile `opar_cnt` will be decreased
	// each time we find a closing-parenthesis
	while (i < d->opar_num)
	{
		if (d->all_open_pars[i][0] < last_cpar_ind &&
			d->all_open_pars[i][1] == NOT_CLOSED_PAR)
		{
			pair_opar_ind = i;
		}
		++i;
	}

	// Add this closing parenthesis index to the
	// list of all parenthesis pairs to match the
	// corresponding opening parenthesis index
	d->pars[pair_opar_ind].second = d->pi;

	// Mark the matched opening-parenthesis as closed
	d->all_open_pars[pair_opar_ind][1] = CLOSED_PAR;
}

void	parser_free(t_parser_data *d)
{
	ops_free(d->ops);
}
