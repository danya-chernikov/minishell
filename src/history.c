#include "history.h"

#include <stdio.h>
#include <stdlib.h>

/* We assume lines is located on heap */
void	history_push(t_history *history, char *line, t_hist_type type)
{
	history->lines[history->lines_num].type = type;
	history->lines[history->lines_num].cmd = line;
	++history->lines_num;
}

void	history_pop(t_history *history)
{
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
	history->lines = malloc(MAX_HIST_LINES_NUM * sizeof *history->lines);
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

	i = 0;
	while (i < history->lines_num)
	{
		if (history->lines[i].cmd)
		{
			free(history->lines[i].cmd);
			history->lines[i].cmd = NULL; // Does not make much sense here.. but ok
		}
		++i;
	}
	if (history->lines)
		free(history->lines);
}
