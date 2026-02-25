/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:33:10 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 19:34:36 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operand.h"
#include "env.h"

#include "libft.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* Assign the default value to
 * the pipes of all operators */
int	ops_init(t_operand *ops)
{
	int	i;
	int	j;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		ops[i].name = NULL;
		ops[i].write_end = DEFAULT_FD;
		ops[i].read_end = DEFAULT_FD;
		ops[i].tokens = NULL;
		ops[i].my_env = NULL;
		ops[i].f_per_cmd = false;
		j = 0;
		while (j < MAX_REDIRS_NUM)
		{
			ops[i].redirs[j].path = NULL;
			ops[i].redirs[j].hd.content = NULL;
			ops[i].redirs[j].hd.delim = NULL;
			++j;
		}
		++i;
	}
	return (COMMON_SUCCESS);
}

/* Allocates memory for tokens array
 * of the operand `op` */
int	op_token_init(t_operand *op)
{
	size_t	i;

	op->tokens = malloc(MAX_OP_TOKENS_NUM * sizeof (t_op_token));
	if (!op->tokens)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	i = 0;
	while (i < MAX_OP_TOKENS_NUM)
	{
		op->tokens[i].cnt = NULL;
		op->tokens[i].qpair_cnt = 0;
		++i;
	}
	return (COMMON_SUCCESS);
}

int	op_env_init(t_operand *op)
{
	int	fres;

	op->my_env = malloc(1 * sizeof (t_env));
	if (!op->my_env)
		return (COMMON_SYS_ERR);
	fres = env_init(op->my_env, NULL);
	if (fres != COMMON_SUCCESS)
	{
		free(op->my_env);
		op->my_env = NULL;
		return (fres);
	}
	return (COMMON_SUCCESS);
}

