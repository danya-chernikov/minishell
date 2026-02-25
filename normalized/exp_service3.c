/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_service3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:32:23 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 19:32:24 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "operand.h"
#include "prompt_parser.h"

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>

int	exp_alloc_argv(t_operand *op)
{
	op->argv = (char **)ft_calloc(MAX_ARGC_NUM, sizeof (char *));
	if (!op->argv)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* This function we're actually gonna use
 * in exec_ops() after launching execve()
 * when we don't need argv[] of this
 * operand anymore */
void	exp_free_argv(t_operand	*op)
{
	int	arg_i;

	if (!op)
		return ;
	if (op->argv)
	{
		arg_i = 0;
		while (arg_i < op->argc)
		{
			free(op->argv[arg_i]);
			op->argv[arg_i] = NULL;
			++arg_i;
		}
		free(op->argv);
		op->argv = NULL;
	}
	op->argc = 0;
}

/* Frees argv[] of all operands */
void	exp_free_all_ops_argv(t_parser_data *d)
{
	size_t		op_i;
	int			arg_i;
	t_operand	*op;

	op_i = 0;
	while (op_i < d->op_cnt)
	{
		op = &d->ops[op_i];
		if (op->argv)
		{
			arg_i = 0;
			while (arg_i < op->argc)
			{
				free(op->argv[arg_i]);
				op->argv[arg_i] = NULL;
				++arg_i;
			}
			free(op->argv);
			op->argv = NULL;
		}
		op->argc = 0;
		++op_i;
	}
}

void	exp_free_op_tokens(t_operand *op)
{
	size_t	i;

	if (!op)
		return ;
	i = 0;
	while (i < op->token_cnt)
	{
		free(op->tokens[i].cnt);
		op->tokens[i].cnt = NULL;
		op->tokens[i].qpair_cnt = 0;
		op->tokens[i].redir_ind = -1;
		++i;
	}
	op->token_cnt = 0;
}
