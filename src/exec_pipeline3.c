/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:04:14 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:04:14 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "operand.h"
#include "aux_common.h"
#include "signals.h"

#include "error.h"

#include <unistd.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>

int	pl_alloc(t_pipeline *pl)
{
	pl->stages = malloc(pl->stages_num * sizeof (*pl->stages));
	if (!pl->stages)
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	pl->pids = malloc(pl->stages_num * sizeof (*pl->pids));
	if (!pl->pids)
	{
		free(pl->stages);
		pl->stages = NULL;
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	}
	pl->pipes = NULL;
	if (pl->stages_num > 1)
	{
		pl->pipes = malloc((pl->stages_num - 1) * sizeof (*pl->pipes));
		if (!pl->pipes)
		{
			free(pl->stages);
			free(pl->pids);
			pl->pids = NULL;
			pl->stages = NULL;
			return (perror_and_return("malloc", COMMON_SYS_ERR));
		}
	}
	return (COMMON_SUCCESS);
}

void	pl_free(t_pipeline *pl)
{
	if (pl->pipes)
		free(pl->pipes);
	if (pl->pids)
		free(pl->pids);
	if (pl->stages)
		free(pl->stages);
}

int	pl_close_all_pipes(t_pipeline *pl)
{
	int		pi;
	bool	f_failed;

	if (!pl->pipes)
		return (COMMON_SUCCESS);
	pi = 0;
	f_failed = false;
	while (pi < pl->stages_num - 1)
	{
		if (close(pl->pipes[pi][READ_END]) == -1)
		{
			perror("close");
			f_failed = true;
		}
		if (close(pl->pipes[pi][WRITE_END]) == -1)
		{
			perror("close");
			f_failed = true;
		}
		++pi;
	}
	if (f_failed)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

int	pl_wait(t_pipeline *pl)
{
	int	pid_i;
	int	ws;
	int	last;

	last = 0;
	pid_i = 0;
	while (pid_i < pl->stages_num)
	{
		if (waitpid(pl->pids[pid_i], &ws, 0) == -1)
			return (COMMON_SYS_ERR);
		if (pid_i == pl->stages_num - 1)
			last = pl_wait_status(ws);
		++pid_i;
	}
	return (last);
}

int	pl_wait_status(int ws)
{
	int	signum;

	if (WIFEXITED(ws))
		return (WEXITSTATUS(ws));
	if (WIFSIGNALED(ws))
	{
		signum = WTERMSIG(ws);
		if (signum == SIGQUIT)
			write(STDERR_FILENO, SIGQUIT_MSG, 19);
		else if (signum == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		return (SIGNALED_CODE + signum);
	}
	return (COMMON_SUCCESS);
}
