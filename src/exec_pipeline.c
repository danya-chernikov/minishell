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
 * sig[1] - old_quit */
int	pl_exec_pipeline(t_shell *msh, size_t l, size_t r, int depth)
{
	t_pipeline			pl;
	t_parser_data		*pd;
	int					status;
	struct sigaction	sig[2];

	pd = msh->pd;
	ft_bzero(&pl, sizeof (pl));
	pl.stages_num = pl_count_stages(pd, l, r);
	if (pl.stages_num < 0)
		return (RET_CMD_FAILURE);
	if (pl.stages_num == 0)
		return (RET_CMD_SUCCESS);
	if (pl.stages_num == 1 && depth == 0 && l == r &&
		pd->tokens[l].type == OPERAND)
	{
		status = pl_parent_single_try(msh, &pd->tokens[l], depth);
		if (status != NOT_EXEC_IN_PARENT)
			return (status);
	}
	if (pl_alloc(&pl) != COMMON_SUCCESS)
		return (RET_CMD_FAILURE);
	if (pl_fill_stages(pd, l, r, &pl) != COMMON_SUCCESS)
		return (pl_free(&pl), RET_CMD_FAILURE);
	if (pl_make_pipes(&pl) != COMMON_SUCCESS)
		return (pl_free(&pl), RET_CMD_FAILURE);

	parent_ignore_sigint_sigquit(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);

	if (pl_spawn_all(msh, &pl, depth) != COMMON_SUCCESS)
	{
		parent_restore_signals(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
		pl_free(&pl);
		return (pl_free(&pl), RET_CMD_FAILURE);
	}
	status = pl_wait(&pl);
	parent_restore_signals(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
	pl_free(&pl);
	return (status);
}

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

/* NEW */
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

/* OLD */
/*int	pl_spawn_all(t_shell *msh, t_pipeline *pl, int depth, struct sigaction *sig)
{
	int	fret;
	int	st_i;
	
	parent_ignore_sigint_sigquit(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
	st_i = 0;
	while (st_i < pl->stages_num)
	{
		fret = pl_fork_one_stage(msh, pl, st_i, depth);
		if (fret != COMMON_SUCCESS)
		{
			if (pl_close_all_pipes(pl) != COMMON_SUCCESS)
				return (COMMON_SYS_ERR);
			parent_restore_signals(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
			return (fret);
		}
		++st_i;
	}
	if (pl_close_all_pipes(pl) != COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	parent_restore_signals(&sig[SIG_OLD_INT], &sig[SIG_OLD_QUIT]);
	return (COMMON_SUCCESS);
}*/

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
		{
			pl->stages[st_i].l = j;
			pl->stages[st_i].r = j;
			++st_i;
			++j;
		}
		else if (pd->tokens[j].type == OPEN_PAR)
		{
			close_i = pl_find_close(pd, j, r);
			if (close_i == CLOSE_PAR_NOT_FOUND)
				return (COMMON_FAILURE);
			pl->stages[st_i].l = j;
			pl->stages[st_i].r = (size_t)close_i;
			++st_i;
			j = (size_t)close_i + 1;
		}
		else
			++j;
	}
	if (st_i != pl->stages_num)
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}
