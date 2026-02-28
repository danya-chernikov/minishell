/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_preparation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:54:59 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:59:04 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static bool	is_redir_split(t_operand *op, size_t i);
static int	push_redir_token(t_operand *op, size_t *i, size_t len);
static int	push_word_token(t_operand *op, size_t *i, size_t len);
static int	create_token(t_operand *op, size_t start, size_t tok_len);

/* Divide operand->name into op->tokens[].cnt by
 * spaces outside quotes. Allocates each token
 * with exact length (tok_len + 1). On any error
 * frees everything it already allocated via
 * exp_free_op_tokens() */
int	exp_divide_op_str_on_tokens(t_operand *op)
{
	size_t	i;
	size_t	len;
	int		res;

	if (!op || !op->name || !op->tokens)
		return (COMMON_FAILURE);
	op->token_cnt = 0;
	i = 0;
	len = ft_strlen(op->name);
	while (i < len)
	{
		while (i < len && op->name[i] == ' ' && !is_inside_op_quotes(op, i))
			++i;
		if (i >= len)
			break ;
		if (is_redir_split(op, i))
			res = push_redir_token(op, &i, len);
		else
			res = push_word_token(op, &i, len);
		if (res != COMMON_SUCCESS)
			return (exp_free_op_tokens(op), res);
	}
	return (COMMON_SUCCESS);
}

static bool	is_redir_split(t_operand *op, size_t i)
{
	if (is_inside_op_quotes(op, i))
		return (false);
	if (op->name[i] == '>' || op->name[i] == '<')
		return (true);
	return (false);
}

static int	push_redir_token(t_operand *op, size_t *i, size_t len)
{
	size_t	tok_len;

	tok_len = 1;
	if (*i + 1 < len
		&& op->name[*i + 1] == op->name[*i]
		&& !is_inside_op_quotes(op, *i + 1))
	{
		tok_len = 2;
	}
	if (create_token(op, *i, tok_len) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	*i += tok_len;
	return (COMMON_SUCCESS);
}

static int	push_word_token(t_operand *op, size_t *i, size_t len)
{
	size_t	start;

	start = *i;
	while (*i < len)
	{
		if (!is_inside_op_quotes(op, *i) && op->name[*i] == ' ')
			break ;
		if (is_redir_split(op, *i))
			break ;
		++(*i);
	}
	if (create_token(op, start, *i - start) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}

static int	create_token(t_operand *op, size_t start, size_t tok_len)
{
	char	*tok;

	if (op->token_cnt >= MAX_OP_TOKENS_NUM)
	{
		print_shell_error(NULL, MAX_OP_TOK_NUM_ERR_MSG);
		return (COMMON_FAILURE);
	}
	if (tok_len >= MAX_OP_TOKEN_LEN)
	{
		print_shell_error(NULL, MAX_OP_TOK_LEN_ERR_MSG);
		return (COMMON_FAILURE);
	}
	tok = (char *)malloc((tok_len + 1) * sizeof(char));
	if (!tok)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	ft_memcpy(tok, op->name + start, tok_len);
	tok[tok_len] = '\0';
	op->tokens[op->token_cnt].cnt = tok;
	op->tokens[op->token_cnt].qpair_cnt = 0;
	op->tokens[op->token_cnt].redir_ind = -1;
	++op->token_cnt;
	return (COMMON_SUCCESS);
}
