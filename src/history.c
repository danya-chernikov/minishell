#include "history.h"

#include <stdio.h>
#include <stdlib.h>

int	history_init(t_history *history)
{
	size_t	i;

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
