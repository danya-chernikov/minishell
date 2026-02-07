/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 23:09:35 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/08 00:23:56 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "shell.h"
#include "operand.h"
#include "token.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>

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

			fret = divide_op_str_on_tokens(op);
			if (fret != COMMON_SUCCESS)
				return (fret);

			fret = update_op_tokens_quote_intervals(op);
			if (fret != COMMON_SUCCESS)
				return (fret);

			opt_i = 0;
			while (opt_i < op->token_cnt)
			{
				op_tok = &op->tokens[opt_i];
				if (token_is_redirect(op_tok))
				{
					// Just skip this operand token
					++opt_i;
					continue ;
				}
				else if (token_is_assignment(op, op_tok)) // Assignment
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
						//t_ind_type		state;
						size_t			i;
						size_t			tok_len;
						char			*tok_str;

						tok_str = op_tok->cnt;

						f_varname_correct = true;
						// We're gonna treat this token as an assignment
						
						//state = IND_QNONE;

						// We do not have to expand anything in variable's name
						// So let's start with the next symbol after '='	
						// Always search for the first '=' occurence
						i = ft_abs((ft_strchr(tok_str, '=') - tok_str));
						tok_len = ft_strlen(tok_str);

						// We actually can use op_tok->quotes because if we reach this point
						// it means ALL quote intervals are located after the first '=' (unquoted)

						// Let's define qmask[] and the new array.
						// They both are gonna grow dynamically	

						// Loop through token symbols
						while (i < tok_len)
						{
							// Again, we already excluded cases like VAR"IABLE=VALUE"
							// here the variable name will not be considered as correct
							// and we'll be treating this token as an argument
							
							// If current symbol is a double syntax quote
							if (tok_str[i] == '"' &&
								is_syntax_quote(op_tok->quotes, op_tok->qpair_cnt, i))
							{

							}
							++i;
						}
					}

					free(var_name);
					free(var_value);

					if (f_varname_correct)
					{
						++opt_i; // Next token
						continue ;
					}
					// If variable's name is not correct
					// We're gonna treat this token as an argument
				}

				// Argument/Redirection's operand
				
				++opt_i;
			}


		} // if (token->type == OPERAND)
	
		++ti;

	} // while (ti < pd->token_cnt)

	return (fret);
}

/* Divide operand's string 'name' on tokens by spaces
 * (which are located outside any quote intervals of
 * course). Each operand contains at least one token */
int	divide_op_str_on_tokens(t_operand *op)
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
int update_op_tokens_quote_intervals(t_operand *op)
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

bool	token_is_assignment(t_operand *op, t_op_token *op_tok)
{
	size_t	i;
	size_t	slen;

	i = 0;
	slen = ft_strlen(op_tok->cnt);
	while (i < slen)
	{
		// If we found = outside any quotes
		if (op_tok->cnt[i] == '=' &&
			!is_inside_quotes_uni(op_tok->quotes, op_tok->qpair_cnt, i))
		{
			if (i > 0) // On the left from '=' there are some symbols
				if (op->argc == 0) // We have not found any argument yet
					return (true);
		}
		++i;
	}
	return (false);
}

bool	token_is_redirect(t_op_token *op_tok)
{
	if (token_is_redir_in(op_tok) || token_is_redir_out(op_tok) ||
		token_is_redir_app(op_tok) || token_is_heredoc(op_tok))
	{
		return (true);
	}
	return (false);
}

bool	token_is_redir_in(t_op_token *op_tok)
{
	if (op_tok->cnt[0] == '<' && op_tok->cnt[1] == '\0')
		return (true);
	return (false);
}

bool	token_is_redir_out(t_op_token *op_tok)
{
	if (op_tok->cnt[0] == '>' && op_tok->cnt[1] == '\0')
		return (true);
	return (false);
}

bool	token_is_redir_app(t_op_token *op_tok)
{
	if (op_tok->cnt[0] == '>' && op_tok->cnt[1] == '>' &&
		op_tok->cnt[2] == '\0')
	{
		return (true);
	}
	return (false);
}

bool	token_is_heredoc(t_op_token *op_tok)
{
	if (op_tok->cnt[0] == '<' && op_tok->cnt[1] == '<' &&
		op_tok->cnt[2] == '\0')
	{
		return (true);
	}
	return (false);
}

int	close_pipes(t_shell *msh)
{
	size_t	i;

	i = 0;
	while (i < msh->pd->pipe_cnt)
	{
		if (close(msh->pd->pipes[i][READ_END]) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
		if (close(msh->pd->pipes[i][WRITE_END]) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
		++i;
	}
	return (COMMON_SUCCESS);
}
