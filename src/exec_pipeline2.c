#include "exec.h"
#include "shell.h"
#include "operand.h"
#include "builtin.h"
#include "prompt_parser.h"

#include "error.h"

#include <stdlib.h>

int	pl_fork_one_stage(t_shell *msh, t_pipeline *pl, int st_i, int depth)
{
	int	in_fd;
	int	out_fd;

	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	if (st_i != 0)
		in_fd = pl->pipes[st_i - 1][READ_END];
	if (st_i != pl->stages_num - 1)
		out_fd = pl->pipes[st_i][WRITE_END];
	pl->pids[st_i] = fork();
	if (pl->pids[st_i] == -1)
		return (COMMON_SYS_ERR);
	if (pl->pids[st_i] == 0)
	{
		child_set_default_signals();
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);
		if (pl_close_all_pipes(pl) != COMMON_SUCCESS)
			exit (RET_CMD_FAILURE);
		if (msh->pd->tokens[pl->stages[st_i].l].type == OPERAND)
			child_exec_operand(msh, &msh->pd->tokens[pl->stages[st_i].l]);
		pl_child_run_subshell(msh, pl->stages[st_i], depth);
	}
	return (COMMON_SUCCESS);
}

/* We pass `stage` by value cause it's small and doesn't
 * have to return changes outside. Moreover after fork()
 * the common memory will not exist anyway*/
void	pl_child_run_subshell(t_shell *msh, t_range stage, int depth)
{
	int	ret;

	ret = exec_ops_range(msh, stage.l + 1, stage.r - 1, depth + 1);
	exit(ret);
}

int	pl_count_stages(t_parser_data *pd, size_t l, size_t r)
{
	size_t	j;
	int		st_cnt;
	int		close_i;

	j = l;
	st_cnt = 0;
	while (j <= r)
	{
		if (pd->tokens[j].type == OPERAND)
		{
			++st_cnt;
			++j;
		}
		else if (pd->tokens[j].type == OPEN_PAR)
		{
			close_i = pl_find_close(pd, j, r);
			if (close_i == CLOSE_PAR_NOT_FOUND)
				return (-1);
			++st_cnt;
			j = (size_t)close_i + 1;
		}
		else
			++j;
	}
	return (st_cnt);
}

/* d - parentheses depth */
int	pl_find_close(t_parser_data *pd, size_t open_i, size_t r)
{
	size_t	j;
	int		d;

	j = open_i;
	d = 0;
	while (j <= r)
	{
		if (pd->tokens[j].type == OPEN_PAR)
			++d;
		else if (pd->tokens[j].type == CLOSE_PAR)
		{
			--d;
			if (d == 0)
				return ((int)j);
		}
		++j;
	}
	return (CLOSE_PAR_NOT_FOUND);
}

int	pl_make_pipes(t_pipeline *pl)
{
	int	pi;

	if (pl->stages_num <= 1)
		return (COMMON_SUCCESS);
	pi = 0;
	while (pi < pl->stages_num - 1)
	{
		if (pipe(pl->pipes[pi]) == -1)
			return (COMMON_SYS_ERR);
		++pi;
	}
	return (COMMON_SUCCESS);
}
