#include "exec.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"
#include "token.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int	exec_ops(t_shell *msh, int *ret_code)
{
	(void)ret_code;

	int				fret;
	size_t			ti; // Token index

	t_parser_data	*pd;
	
	ti = 0;
	pd = msh->pd;
	fret = COMMON_SUCCESS;
	// Go through all token-operands
	while (ti < pd->token_cnt)
	{
		t_token		*token;
		t_operand	*op;

		token = &pd->tokens[ti];
		if (token->type == OPERAND)
		{
			size_t		opt_i; // Operand's token index
			t_op_token	*op_tok;

			op = token->op;

			fret = exec_divide_op_str_on_tokens(op);
			if (fret != COMMON_SUCCESS)
				return (fret);

			fret = exec_update_op_tokens_quote_intervals(op);
			if (fret != COMMON_SUCCESS)
				return (fret);

			opt_i = 0;
			while (opt_i < op->token_cnt)
			{
				op_tok = &op->tokens[opt_i];

				// >, >>, <, <<
				if (exec_token_is_redirect(op_tok))
				{
					++opt_i; // Just skip this operand token
					continue ;
				}
				else if (exec_token_is_assignment(op, op_tok)) // Assignment
				{
					// Split operand's token string by '='
					char	*var_name;	
					char	*var_value;
					bool	f_varname_correct;

					f_varname_correct = false;

					// Here we consider only the first '=' found
					// If '=' was located by token_is_assignment() it was the first '='
					// VAR'='=1 in this case we'll get VAR' and '=1 but the variable name simply
					// will not be considered as correct later! So it's fine
					// VAR==1 we'll just get VAR and =1
					fret = div2_str_by_delim(op_tok->cnt, '=', &var_name, &var_value);
					if (fret != COMMON_SUCCESS)
						return (fret);

					// Check LHS (variable's name) for correctness
					if (is_variable_name_correct(var_name)) // If correct
					{
						t_ind_type		state;			// State of the current operand's token symbol
						size_t			i;				// Symbol index in operand's token
						size_t			eqsign_ind;
						size_t			tok_len;
						char			*tok_str;		// Pointer to operand's token string
						t_vector		qmask;			// Quote mask
						t_vector		exp_res;		// Array that stores expansion results
														// (the same as the index for `qmask`)
						t_vector		*vec_pair[2];	// To pass our lovely friend Norminette
							
						tok_str = op_tok->cnt;

						f_varname_correct = true;
						// We're gonna treat this token as an assignment
						
						state = IND_QNONE;

						tok_len = ft_strlen(tok_str);

						// We do not have to expand anything in variable's name
						// So let's start with the next symbol after '='	
						// Always search for the first '=' occurence
						eqsign_ind = ft_abs(ft_strchr(tok_str, '=') - tok_str);
						i = eqsign_ind + 1;

						// If after '=' goes nothing ( things like "VAR=" )
						if (i == tok_len)
						{
							// We don't have to expand anything
							// Let's create a variable with an empty value
							// We don't care about an error cause it can't be critical
							env_set(op->my_env, var_name, var_value, LOCAL);
							free(var_name);
							free(var_value);
							++opt_i;
							continue ; // Go to the next operand's token
						}

						// Initialize `qmask` and `exp_res` vectors
						if (!exec_vectors_init(&qmask, &exp_res, tok_len - i + 1))
						{
							free(var_name);
							free(var_value);
							return (COMMON_SYS_ERR);
						}

						vec_pair[0] = &exp_res;
						vec_pair[1] = &qmask;

						// We actually can use op_tok->quotes because if we reach this point
						// it means ALL quote intervals are located after the first '=' (unquoted)

						// Loop through token symbols
						i = 0;
						while (i < tok_len)
						{
							// Again, we already excluded cases like VAR"IABLE=VALUE"
							// here the variable name will not be considered as correct
							// and we'll be treating this token as an argument
							
							if (tok_str[i] == '"') // If current symbol is a double syntax quote
							{
								exec_process_double_quote(tok_str, &i, vec_pair, state);
							}
							else if (tok_str[i] == '\'') // If current symbol is a single syntax quote
							{
								exec_process_single_quote(tok_str, &i, vec_pair, state);
							}
							else if (exec_tilde_found(tok_str, i, eqsign_ind, state)) // If ~ was found
							{
								exec_expand_tilde(msh, vec_pair, state);
							}
							else if (tok_str[i] == '$' && state != IND_QSINGLE) // If $ was found
							{
								exec_expand_variable(msh, vec_pair, var_name, state);
							}
							else // Current symbol is just a regular symbol (except quote)
							{
								vector_push_back_char(&exp_res, tok_str[i]);
								vector_push_back_char(&qmask, (char)state);
							}
							++i;
						} // while (i < tok_len)

					} // if (is_variable_name_correct(var_name))

					// Create a local environment variable and add it into
					// the array of variables of this operand `vars`
					// ...
				

					free(var_name);
					free(var_value);

					if (f_varname_correct)
					{
						++opt_i; // Next token
						continue ;
					}
					// If variable's name is not correct
					// We're gonna treat this token as an argument

				} // else if (token_is_assignment(op, op_tok))

				// Argument/Redirection's operand
				// Current operand's token is a regular argument or a redirection opernad
				// ...

				++opt_i;
			} // while (opt_i < op->token_cnt)


		} // if (token->type == OPERAND)
	
		++ti;

	} // while (ti < pd->token_cnt)

	return (fret);
}

/* Divide operand's string 'name' on tokens by spaces
 * (which are located outside any quote intervals of
 * course). Each operand contains at least one token */
int	exec_divide_op_str_on_tokens(t_operand *op)
{
	size_t		op_i;
	size_t		ti;
	size_t		opstr_len;
	char		*op_str;
	char		*tcnt; // Operand's token content

	op_str = op->name;
	opstr_len = ft_strlen(op_str);

	tcnt = (char *)malloc(MAX_OP_TOKEN_LEN * sizeof(char));
	if (!tcnt)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	ti = 0;
	op_i = 0;
	while (op_i < opstr_len)
	{
		if (op_i > MAX_OP_TOKEN_LEN - 1) // Overflow check
		{
			print_shell_error(NULL, MAX_OP_TOK_LEN_ERR_MSG);
			return (COMMON_FAILURE);
		}
		if (op_str[op_i] == ' ' && !is_inside_op_quotes(op, op_i))
		{
			if (op->token_cnt > MAX_OP_TOKENS_NUM - 1) // Overflow check
			{
				print_shell_error(NULL, MAX_OP_TOK_NUM_ERR_MSG);
				return (COMMON_FAILURE);
			}
			tcnt[ti] = '\0';
			ti = 0;
			op->tokens[op->token_cnt].cnt = tcnt;
			++op->token_cnt;
			tcnt = (char *)malloc(MAX_OP_TOKEN_LEN * sizeof(char));
			if (!tcnt)
			{
				perror("malloc");
				return (COMMON_SYS_ERR);
			}
			skip_spaces(op_str, &op_i);
			continue ;
		}
		tcnt[ti] = op_str[op_i];
		++op_i;
		++ti;
	}
	tcnt[ti] = '\0';
	op->tokens[op->token_cnt].cnt = tcnt;
	++op->token_cnt;
	return (COMMON_SUCCESS);
}

/* Updates quote intervals for each operand's token */
int exec_update_op_tokens_quote_intervals(t_operand *op)
{
	int			fret;
	size_t		ti;
	t_op_token	*op_tok;

	ti = 0;
	fret = COMMON_SUCCESS;
	while (ti < op->token_cnt)
	{
		op_tok = &op->tokens[ti];
		fret = quotes_parser(op_tok->cnt, op_tok->quotes, &op_tok->qpair_cnt);
		if (fret != COMMON_SUCCESS)
			return (fret);
		++ti;
	}
	return (fret);
}

void	exec_process_double_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type state)
{
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[0];
	qmask = vec_pair[1];
	if (state == IND_QNONE) // We enter the double quotation interval
	{
		state = IND_QDOUBLE;
	}
	else if (state == IND_QDOUBLE) // We left the double quotation interval
	{
		state = IND_QNONE;
	}
	else if (state == IND_QSINGLE) // This double quote is just a data quote
	{
		// Copy this double quote (cause it's just a regular symbol)
		vector_push_back_char(exp_res, tok_str[*i]);
		// Mark the corresponding index in qmask[] as IND_QSINGLE
		vector_push_back_char(qmask, (char)IND_QSINGLE);
	}
}

void	exec_process_single_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type state)
{
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[0];
	qmask = vec_pair[1];
	if (state == IND_QNONE) // We enter the single quotation interval
	{
		state = IND_QSINGLE;
	}
	else if (state == IND_QSINGLE) // We left the single quotation interval
	{
		state = IND_QNONE;
	}
	else if (state == IND_QDOUBLE) // This single quote is just a data quote
	{
		// Copy this single quote (cause it's just a regular symbol)
		vector_push_back_char(exp_res, tok_str[*i]);
		// Mark the corresponding index in qmask[] as IND_QDOUBLE
		vector_push_back_char(qmask, (char)IND_QDOUBLE);
	}
}
