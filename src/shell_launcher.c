/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launcher.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 02:18:38 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 03:54:27 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "engine.h"
#include "signals.h"

#include "error.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* If everything is OK, we return the exit code
 * of the last command executed by the shell
 * (regardless of which mode it was launched in).
 * If a system error occurs, we return -1
 * (COMMON_SYS_ERR). COMMON_SUCCESS is treated as
 * a normal command return value, so we don't
 * handle it specially. In main(), if the function
 * returns COMMON_SYS_ERR, we return 1, otherwise,
 * main() simply returns the exit code of the last
 * executed command, in any shell mode */
int	msh_launch(t_shell *msh, int *ret_code)
{
	int	fres;

	if (msh->mode == NONINT_SCRIPT_MODE)
		fres = launch_script(msh, ret_code);
	else if (msh->mode == NONINT_CMD_MODE)
		fres = launch_cmd(msh, ret_code);
	else if (msh->mode == NONINT_STDIN_MODE)
		fres = launch_stdin_cmd(msh, ret_code);
	else if (msh->mode == INT_MODE)
		fres = launch_int_session(msh, ret_code);
	return (fres);
}

/* Just execute the command 
 * that goes after -c and exit */
int	launch_cmd(t_shell *msh, int *ret_code)
{
	int	fres;

	*ret_code = EXIT_SUCCESS;
	fres = shell_engine(msh, msh->c_cmd, ret_code);
	return (fres);
}

/* Read commands from stdin and 
 * exexute them.
 * DO WE NEED TO PROCESS Ctrl+C by
 * ourselves here? */
int	launch_stdin_cmd(t_shell *msh, int *ret_code)
{
	int		fres;
	int		gnlerr;
	char	*line;

	fres = COMMON_SUCCESS;
	*ret_code = EXIT_SUCCESS;
	line = get_next_line(STDIN_FILENO, &gnlerr);
	gnlerr = 0;
	while (line)
	{
		remove_newline(line);
		if (msh->opts.f_verbose)
			printf("%s\n", line);
		fres = shell_engine(msh, line, ret_code);
		free(line);
		if (fres != COMMON_SUCCESS)
			return (gnl_finish(STDIN_FILENO), fres);
		line = get_next_line(STDIN_FILENO, &gnlerr);
	}
	if (!line && gnlerr)
	{
		print_shell_error(NULL, GNL_ERR_MSG);
		return (gnl_finish(STDIN_FILENO), COMMON_SYS_ERR);
	}
	return (fres);
}
