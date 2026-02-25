/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_login_configs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:08:37 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 12:40:05 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "engine.h"

#include "error.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int	load_login_configs_loop(t_shell *msh, int *fd);

/* Always opens /etc/profile. Then,
 * consecutively tries to open:
 *     ~/.minishell_profile
 *     ~/.minishell_login
 *     ~/.profile
 * Commands are executed from the first
 * file that is successfully opened */
int	load_login_configs(t_shell *msh)
{
	int		fret;
	int		fd;
	int		cnf_i;

	cnf_i = 0;
	while (cnf_i < LOGIN_CONFIGS_NUM - 2)
	{
		if (access(msh->configs.login[cnf_i], F_OK) == -1)
		{
			++cnf_i;
			continue ;
		}
		fd = open(msh->configs.login[cnf_i], O_RDONLY);
		if (fd == -1)
			return (perror("open"), COMMON_SYS_ERR);
		fret = load_login_configs_loop(msh, &fd);
		if (fret != COMMON_SUCCESS)
			return (fret);
		if (close(fd) == -1)
			return (perror("close"), COMMON_SYS_ERR);
		if (cnf_i > 0)
			break ;
		++cnf_i;
	}
	return (COMMON_SUCCESS);
}

static int	load_login_configs_loop(t_shell *msh, int *fd)
{
	int		gnlerr;
	int		ret_code;
	char	*line;

	gnlerr = 0;
	line = get_next_line(*fd, &gnlerr);
	while (line)
	{
		remove_newline(line);
		if (msh->opts.f_verbose)
			printf("%s\n", line);
		shell_engine(msh, NULL, &ret_code);
		free(line);
		gnlerr = 0;
		line = get_next_line(*fd, &gnlerr);
	}
	if (!line && gnlerr)
	{
		print_shell_error(NULL, GNL_ERR_MSG);
		gnl_finish(*fd);
		if (close(*fd) == -1)
			return (perror("close"), COMMON_SYS_ERR);
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
