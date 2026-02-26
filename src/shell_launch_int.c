/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launch_int.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 03:21:07 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 22:35:59 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "engine.h"
#include "signals.h"
#include "aux_common.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int	process_prompt(t_shell *msh, char *rline_buf, int *ret_code);

/* Create an interactive shell session
 * and ask user for commands showing
 * prompt invitation */
int	launch_int_session(t_shell *msh, int *ret_code)
{
	int		fres;
	char	*rline_buf;

	signals_init();
	if (msh_load_history(msh) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	fres = COMMON_SUCCESS;
	*ret_code = EXIT_SUCCESS;
	rline_buf = NULL;
	while (1)
	{
		fres = gen_prompt_inv(msh);
		if (fres != COMMON_SUCCESS)
			return (fres);
		rl_done = 0;
		g_got_sigint = 0;
		rline_buf = readline(msh->prompt_inv);
		fres = process_prompt(msh, rline_buf, ret_code);
		if (fres == BREAK || fres == COMMON_SYS_ERR)
			break ;
		else if (fres == CONTINUE)
			continue ;
	}
	return (fres);
}

static int	process_prompt(t_shell *msh, char *rline_buf, int *ret_code)
{
	int	fres;

	if (!rline_buf)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (BREAK);
	}
	if (g_got_sigint)
	{
		g_got_sigint = 0;
		*ret_code = SIGNALED_CODE + SIGINT;
	}
	if (rline_buf[0] == '\0')
	{
		free(rline_buf);
		rline_buf = NULL;
		return (CONTINUE);
	}
	add_history(rline_buf);
	if (msh->opts.f_verbose)
		printf("%s\n", rline_buf);
	fres = shell_engine(msh, rline_buf, ret_code);
	free(rline_buf);
	rline_buf = NULL;
	return (fres);
}
