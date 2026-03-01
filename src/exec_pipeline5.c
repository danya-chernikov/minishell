/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:00:50 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:00:50 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "shell.h"
#include "builtin.h"
#include "signals.h"
#include "prompt_parser.h"

#include "libft.h"
#include "error.h"

static int	pl_prepare_alloc(t_parser_data *pd, t_pipeline *pl,
				size_t l, size_t r);
static int	pl_run_pipeline(t_shell *msh, t_pipeline *pl, int depth);

/* sig[0] - old_int
 * sig[1] - old_quit */
int	pl_exec_pipeline(t_shell *msh, size_t l, size_t r, int depth)
{
	t_pipeline	pl;
	int			status;

	ft_bzero(&pl, sizeof(pl));
	pl.stages_num = pl_count_stages(msh->pd, l, r);
	if (pl.stages_num < 0)
		return (RET_CMD_FAILURE);
	if (pl.stages_num == 0)
		return (RET_CMD_SUCCESS);
	if (pl.stages_num == 1 && depth == 0 && l == r
		&& msh->pd->tokens[l].type == OPERAND)
	{
		status = pl_parent_single_try(msh, &msh->pd->tokens[l], depth);
		if (status != NOT_EXEC_IN_PARENT)
			return (status);
	}
	status = pl_prepare_alloc(msh->pd, &pl, l, r);
	if (status == COMMON_SUCCESS)
		status = pl_run_pipeline(msh, &pl, depth);
	pl_free(&pl);
	return (status);
}

static int	pl_prepare_alloc(t_parser_data *pd, t_pipeline *pl,
				size_t l, size_t r)
{
	if (pl_alloc(pl) != COMMON_SUCCESS)
		return (RET_CMD_FAILURE);
	if (pl_fill_stages(pd, l, r, pl) != COMMON_SUCCESS)
		return (pl_free(pl), RET_CMD_FAILURE);
	if (pl_make_pipes(pl) != COMMON_SUCCESS)
		return (pl_free(pl), RET_CMD_FAILURE);
	return (COMMON_SUCCESS);
}

static int	pl_run_pipeline(t_shell *msh, t_pipeline *pl, int depth)
{
	int					status;
	struct sigaction	sig[2];

	parent_ignore_sigint_sigquit(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
	if (pl_spawn_all(msh, pl, depth) != COMMON_SUCCESS)
	{
		parent_restore_signals(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
		return (RET_CMD_FAILURE);
	}
	status = pl_wait(pl);
	parent_restore_signals(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
	return (status);
}
