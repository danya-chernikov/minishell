/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:31:05 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:31:06 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "shell.h"
#include "builtin.h"
#include "signals.h"
#include "expansion.h"
#include "aux_common.h"
#include "prompt_parser.h"

#include "error.h"
#include "libft.h"

#include <stdlib.h>

int	pl_parent_single_try(t_shell *msh, t_token *token, int depth)
{
	int	status;

	if (depth != 0 || token->type != OPERAND || token->op == NULL)
		return (NOT_EXEC_IN_PARENT);
	if (prepare_operand(msh, token) != COMMON_SUCCESS)
		return (RET_CMD_FAILURE);
	if (token->op->argc == 0)
	{
		status = parent_run_with_redirs(msh, token);
		if (status == 0)
			env_apply_as_local(&msh->env, token->op->my_env);
		exp_free_argv(token->op);
		return (status);
	}
	if (!is_parent_builtin(token->op->argv[0]))
	{
		exp_free_argv(token->op);
		return (NOT_EXEC_IN_PARENT);
	}
	status = parent_run_with_redirs(msh, token);
	exp_free_argv(token->op);
	return (status);
}

int	pl_spawn_all(t_shell *msh, t_pipeline *pl, int depth)
{
	int	fret;
	int	st_i;

	st_i = 0;
	while (st_i < pl->stages_num)
	{
		fret = pl_fork_one_stage(msh, pl, st_i, depth);
		if (fret != COMMON_SUCCESS)
		{
			if (pl_close_all_pipes(pl) != COMMON_SUCCESS)
				return (COMMON_SYS_ERR);
			return (fret);
		}
		++st_i;
	}
	if (pl_close_all_pipes(pl) != COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}
