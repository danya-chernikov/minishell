/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operand2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:33:07 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 19:34:34 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operand.h"
#include "env.h"

#include "libft.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static void	free_tokens_loop(t_operand *ops);
static void	free_redirs_loop(t_operand *ops);

void	ops_free(t_operand *ops)
{
	size_t	i;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		if (ops[i].name)
		{
			free(ops[i].name);
			ops[i].name = NULL;
		}
		if (ops[i].my_env)
		{
			env_free(ops[i].my_env);
			free(ops[i].my_env);
			ops[i].my_env = NULL;
		}
		if (ops[i].tokens)
			free_tokens_loop(ops);
		free_redirs_loop(ops);
		ops[i].red_cnt = 0;
		ops[i].token_cnt = 0;
		ops[i].argc = 0;
		++i;
	}
}

static void	free_tokens_loop(t_operand *ops)
{
	int	j;

	j = 0;
	while (j < MAX_OP_TOKENS_NUM)
	{
		if (ops[i].tokens[j].cnt)
		{
			free(ops[i].tokens[j].cnt);
			ops[i].tokens[j].cnt = NULL;
		}
		++j;
	}
	free(ops[i].tokens);
	ops[i].tokens = NULL;
}

static void	free_redirs_loop(t_operand *ops)
{
	int	j;

	j = 0;
	while (j < MAX_REDIRS_NUM)
	{
		if (ops[i].redirs[j].path)
		{
			free(ops[i].redirs[j].path);
			ops[i].redirs[j].path = NULL;
		}
		if (ops[i].redirs[j].hd.content)
		{
			free(ops[i].redirs[j].hd.content);
			ops[i].redirs[j].hd.content = NULL;
		}
		if (ops[i].redirs[j].hd.delim)
		{
			free(ops[i].redirs[j].hd.delim);
			ops[i].redirs[j].hd.delim = NULL;
		}
		++j;
	}
}
