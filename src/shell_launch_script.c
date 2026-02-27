/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launch_script.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 03:12:00 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 01:48:24 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "engine.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int	launch_script_check_access(t_shell *msh, char *script_path);
static int	launch_script_loop(t_shell *msh, int *fd, int *gnlerr,
				int *ret_code);
static void	apply_verbose_flag(t_shell *msh, char *line);

int	launch_script(t_shell *msh, int *ret_code)
{
	char	*script_path;
	int		gnlerr;
	int		fd;
	int		fres;

	script_path = NULL;
	*ret_code = EXIT_SUCCESS;
	fres = launch_script_check_access(msh, script_path);
	if (fres != COMMON_SUCCESS)
		return (fres);
	fd = open(script_path, O_RDONLY);
	if (fd == -1)
		return (free(script_path), perror("open"), COMMON_SYS_ERR);
	gnlerr = 0;
	fres = launch_script_loop(msh, &fd, &gnlerr, ret_code);
	if (fres != COMMON_SUCCESS)
		return (fres);
	if (close(fd) == -1)
	{
		perror("close");
		fres = COMMON_SYS_ERR;
	}
	return (free(script_path), fres);
}

static int	launch_script_check_access(t_shell *msh, char *script_path)
{
	script_path = get_full_path(msh->script);
	if (!script_path)
		return (COMMON_SYS_ERR);
	if (access(script_path, F_OK) == -1)
	{
		print_shell_error(msh->script, NO_SUCH_FD_ERR_MSG);
		free(script_path);
		return (CMD_NOT_LOCATED_ERR);
	}
	if (access(script_path, R_OK) == -1)
	{
		print_shell_error(msh->script, PERM_DENIED_ERR_MSG);
		free(script_path);
		return (CMD_LOCATED_BUT_NOT_EXEC_ERR);
	}
	return (COMMON_SUCCESS);
}

static int	launch_script_loop(t_shell *msh, int *fd, int *gnlerr,
				int *ret_code)
{
	char	*line;
	int		fres;

	fres = COMMON_SUCCESS;
	line = get_next_line(*fd, gnlerr);
	while (line)
	{
		remove_newline(line);
		apply_verbose_flag(msh, line);
		fres = shell_engine(msh, line, ret_code);
		free(line);
		if (fres != COMMON_SUCCESS)
		{
			gnl_finish(*fd);
			break ;
		}
		line = get_next_line(*fd, gnlerr);
	}
	if (!line && *gnlerr)
	{
		print_shell_error(NULL, GNL_ERR_MSG);
		gnl_finish(*fd);
		fres = COMMON_SYS_ERR;
	}
	return (fres);
}

static void	apply_verbose_flag(t_shell *msh, char *line)
{
	if (msh->opts.f_verbose)
		printf("%s\n", line);
}
