/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_configs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:55:37 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 01:25:22 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "engine.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* In all non-interactive modes,
 * we do NOT read any configs */
int	msh_load_configs(t_shell *msh)
{
	int			fret;
	char		*home;
	t_configs	*c;

	c = &msh->configs;
	home = msh->env.vars[PV_HOME].value;
	if (msh->mode == INT_MODE)
	{
		if (msh->opts.f_login)
		{
			fret = init_login_configs(msh, c, home);
			if (fret != COMMON_SUCCESS)
				return (fret);
		}
		else
		{
			fret = init_nonlogin_configs(msh, c, home);
			if (fret != COMMON_SUCCESS)
				return (fret);
		}
	}
	return (COMMON_SUCCESS);
}

int	init_login_configs(t_shell *msh, t_configs *c, char *home)
{
	size_t	i;

	c->etc_prof_path = expand_homedir(DEF_MSH_ETC_PROF_PATH, home);
	c->login[0] = c->etc_prof_path;
	c->home_prof_path = expand_homedir(DEF_MSH_HOME_PROF_PATH, home);
	c->login[1] = c->home_prof_path;
	c->home_login_path = expand_homedir(DEF_MSH_HOME_LOGIN_PATH, home);
	c->login[2] = c->home_login_path;
	c->home_cmn_prof_path = expand_homedir(DEF_MSH_HOME_CMN_PROF_PATH, home);
	c->login[3] = c->home_cmn_prof_path;
	c->home_logout_path = expand_homedir(DEF_MSH_HOME_LOGOUT_PATH, home);
	c->login[4] = c->home_logout_path;
	c->etc_logout_path = expand_homedir(DEF_MSH_ETC_LOGOUT_PATH, home);
	c->login[5] = c->etc_logout_path;
	i = 0;
	while (i < LOGIN_CONFIGS_NUM)
		if (!c->login[i++])
			return (perror("malloc"), COMMON_SYS_ERR);
	if (load_login_configs(msh) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

int	init_nonlogin_configs(t_shell *msh, t_configs *c, char *home)
{
	c->etc_rc_path = expand_homedir(DEF_MSH_ETC_RC_PATH, home);
	c->nonlogin[0] = c->etc_rc_path;
	c->home_rc_path = expand_homedir(DEF_MSH_HOME_RC_PATH, home);
	c->nonlogin[1] = c->home_rc_path;
	if (!c->etc_rc_path || !c->home_rc_path)
		return (perror("malloc"), COMMON_SYS_ERR);
	if (load_nonlogin_configs(msh) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}
