#include "shell.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* HISTSIZE
 *     1. If 0
 *            Remove all history lines from memory.
 *            History will not be working
 *     2. If N
 *            Show N last commands 
 *     3. If non-numeric
 *            No limit
 *     4. If negative
 *            No limit
 *     5. If not exists
 *            No limit
 *
 * HISTFILESIZE
 *     1. If 0
 *            Remove all content from history file
 *     2. If N
 *            Truncate history file to N lines
 *     3. If non-numeric
 *            No limit
 *     4. If negative
 *            No limit
 *     5. If not exists
 *            No limit */
int msh_load_history(t_shell *msh)
{
	int		fd;
	int		gnlerr;
	char	*hline;
	char	*home;
	
	if (msh->mode == INT_MODE)
	{
		home = msh->env.vars[PV_HOME].value;
		msh->history.histfile_path = expand_homedir(DEF_MSH_HIST_PATH, home);
		if (!msh->history.histfile_path)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		if (access(msh->history.histfile_path, F_OK) == -1) // If not exists
			return (COMMON_SUCCESS);
		fd = open(msh->history.histfile_path, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			return (COMMON_SYS_ERR);
		}
		gnlerr = 0;
		hline = get_next_line(fd, &gnlerr);
		while (hline)
		{
			remove_newline(hline);
			history_push(&msh->history, hline, FROM_FILE);
			hline = get_next_line(fd, &gnlerr);
		}
		if (!hline && gnlerr)
		{
			print_shell_error(NULL, GNL_ERR_MSG);
			gnl_finish(fd);
			if (close(fd) == -1)
			{
				perror("close");
				return (COMMON_SYS_ERR);
			}
			return (COMMON_SYS_ERR);
		}
		if (close(fd) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
	}
	return (COMMON_SUCCESS);
}
