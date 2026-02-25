/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:10:54 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 22:44:52 by dchernik         ###   ########.fr       */
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

/* sig[0] - old_int
 * sig[1] - old_quit

 * Orchestrates the pipeline execution by counting stages, checking
 * for parent-only builtins, and managing the spawning process.*/
int	pl_exec_pipeline(t_shell *msh, size_t l, size_t r, int depth)
{
	t_pipeline			pl;
	int					status;

	ft_bzero(&pl, sizeof (pl));
	pl.stages_num = pl_count_stages(msh->pd, l, r);
	if (pl.stages_num < 0)
		return (RET_CMD_FAILURE);
	if (pl.stages_num == 0)
		return (RET_CMD_SUCCESS);
	if (pl.stages_num == 1 && depth == 0 && l == r
		&& pd->tokens[l].type == OPERAND)
	{
		status = pl_parent_single_try(msh, &pd->tokens[l], depth);
		if (status != NOT_EXEC_IN_PARENT)
			return (status);
	}
	if (pl_alloc(&pl) != COMMON_SUCCESS)
		return (RET_CMD_FAILURE);
	if (pl_fill_stages(pd, l, r, &pl) != COMMON_SUCCESS
		|| pl_make_pipes(&pl) != COMMON_SUCCESS)
		return (pl_free(&pl), RET_CMD_FAILURE);
	status = pl_wrap_execution(msh, &pl, depth);
	pl_free(&pl);
	return (status);
}

/* Handles signal protection during pipeline execution.
** Returns the status after waiting for all child processes. */
static int	pl_wrap_execution(t_shell *msh, t_pipeline *pl, int depth)
{
	int					status;
	struct sigaction	sig[2];

	parent_ignore_sigint_sigquit(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
	if (pl_spawn_all(msh, pl, depth) != COMMON_SUCCESS)
	{
		parent_restore_signals(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
		return (pl_free(pl), RET_CMD_FAILURE);
	}
	status = pl_wait(pl);
	parent_restore_signals(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
	return (status);
}

/* The condition `close_i == CLOSE_PAR_NOT_FOUND` practically should
 * never happen, but just in case let's check it.. */
int	pl_fill_stages(t_parser_data *pd, size_t l, size_t r, t_pipeline *pl)
{
	size_t	j;
	int		st_i;
	int		close_i;

	j = l;
	st_i = 0;
	while (j <= r && st_i < pl->stages_num)
	{
		if (pd->tokens[j].type == OPERAND)
			set_stage_bounds(pl, &st_i, j, j++);
		else if (pd->tokens[j].type == OPEN_PAR)
		{
			close_i = pl_find_close(pd, j, r);
			if (close_i == CLOSE_PAR_NOT_FOUND)
				return (COMMON_FAILURE);
			set_stage_bounds(pl, &st_i, j, (size_t)close_i);
			j = (size_t)close_i + 1;
		}
		else
			++j;
	}
	if (st_i != pl->stages_num)
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}

/* Helper to store the left and right boundaries of a pipeline stage. */
static void	set_stage_bounds(t_pipeline *pl, int *st_i, size_t l, size_t r)
{
	pl->stages[*st_i].l = l;
	pl->stages[*st_i].r = r;
	(*st_i)++;
}
