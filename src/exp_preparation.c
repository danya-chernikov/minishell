/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_preparation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 10:36:55 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 21:02:18 by jhvalenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* Divide operand's string 'name' on tokens by spaces
 * (which are located outside any quote intervals of
 * course). Each operand contains at least one token */
/*int	exp_divide_op_str_on_tokens(t_operand *op)
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
		if (op_i >= MAX_OP_TOKEN_LEN - 1) // Overflow check
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
}*/

/* Divides the operand's name into individual tokens based on spaces.
 * Spaces inside quotes are ignored and treated as part of the token.*/
int	exp_divide_op_str_on_tokens(t_operand *op)
{
	size_t	i;
	size_t	len;
	size_t	start;
	int		res;

	if (!op || !op->name || !op->tokens)
		return (COMMON_FAILURE);
	op->token_cnt = 0;
	len = ft_strlen(op->name);
	i = 0;
	while (i < len)
	{
		while (i < len && op->name == ' ')
			++i;
		if (i >= len)
			break ;
		start = i;
		while (i < len && (op->name[i] != ' ' || is_inside_op_quotes(op, i)))
			++i;
		res = create_token(op, start, i - start);
		if (res != COMMON_SUCCESS)
			return (exp_free_op_tokens(op), res);
	}
	return (COMMON_SUCCESS);
}

/* Allocates and initializes a new token for the operand.
 * Fills the content and sets default values for redirections and quotes.*/
int	create_token(t_operand *op, size_t start, size_t len)
{
	char	*tok;

	if (len >= MAX_OP_TOKEN_LEN)
	{
		print_shell_error(NULL, MAX_OP_TOK_LEN_ERR_MSG);
		return (COMMON_FAILURE);
	}
	if (op->token_cnt >= MAX_OP_TOKENS_NUM)
	{
		print_shell_error(NULL, MAX_OP_TOK_NUM_ERR_MSG);
		return (COMMON_FAILURE);
	}
	tok = (char *)malloc((len + 1) * sizeof(char));
	if (!tok)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	ft_memcpy(tok, op->name + start, len);
	tok[len] = '\0';
	op->tokens[op->token_cnt].cnt = tok;
	op->tokens[op->token_cnt].qpair_cnt = 0;
	op->tokens[op->token_cnt].redir_ind = -1;
	op->token_cnt++;
	return (COMMON_SUCCESS);
}

/* Updates quote intervals for each operand's token */
int	exp_update_op_tokens_quote_intervals(t_operand *op)
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
