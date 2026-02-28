/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:34:15 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 01:14:32 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

#include <stdio.h>
#include <stdlib.h>

/* We assume lines is located on heap */
int	history_push(t_history *history, char *line, t_hist_type type)
{
	if (history->lines_num >= MAX_HIST_LINES_NUM)
	{
		free(line);
		print_shell_error(NULL, MAX_HIST_LNUM_ERR_MSG);
		return (COMMON_FAILURE);
	}
	history->lines[history->lines_num].type = type;
	history->lines[history->lines_num].cmd = line;
	++history->lines_num;
	return (COMMON_SUCCESS);
}

void	history_pop(t_history *history)
{
	if (!history || history->lines_num == 0)
		return ;
	if (history->lines[history->lines_num - 1].cmd)
		free(history->lines[history->lines_num - 1].cmd);
	history->lines[history->lines_num - 1].cmd = NULL;
	--history->lines_num;
}

int	history_init(t_history *history)
{
	size_t	i;

	history->lines_num = 0;
	history->histsize = DEF_HISTSIZE;
	history->histfilesize = DEF_HISTFILESIZE;
	history->lines = malloc(MAX_HIST_LINES_NUM * sizeof (t_hist_cmd));
	history->histfile_path = NULL;
	if (!history->lines)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	i = 0;
	while (i < MAX_HIST_LINES_NUM)
	{
		history->lines[i].cmd = NULL;
		++i;
	}
	return (COMMON_SUCCESS);
}

void	history_free(t_history *history)
{
	size_t	i;

	if (!history)
		return ;
	if (history->lines)
	{
		i = 0;
		while (i < history->lines_num)
		{
			free(history->lines[i].cmd);
			history->lines[i].cmd = NULL;
			++i;
		}
		free(history->lines);
		history->lines = NULL;
	}
	if (history->histfile_path)
	{
		free(history->histfile_path);
		history->histfile_path = NULL;
	}
	history->lines_num = 0;
}
