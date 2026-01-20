#include "shell.h"
#include "engine.h"
#include "debug.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* In all non-interactive modes, we do NOT read any configs */
int	msh_load_configs(t_shell *msh)
{
	int			i;
	char		*home;
	t_configs	*c;

	c = &msh->configs;
	home = msh->env.vars[PV_HOME].value;
	if (msh->mode == INT_MODE)
	{
		if (msh->opts.f_login)
		{
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
			{
				if (!c->login[i])
				{
					perror("malloc");
					return (COMMON_SYS_ERR);
				}
				++i;
			}
#if DEBUG == 1
			dbg_print_configs(c, LOGIN_CONF);
#endif
			if (load_login_configs(msh) == COMMON_SYS_ERR)
				return (COMMON_SYS_ERR);
		}
		else
		{
			c->etc_rc_path = expand_homedir(DEF_MSH_ETC_RC_PATH, home);
			c->nonlogin[0] = c->etc_rc_path;
			c->home_rc_path = expand_homedir(DEF_MSH_HOME_RC_PATH, home);
			c->nonlogin[1] = c->home_rc_path;
			if (!c->etc_rc_path || !c->home_rc_path)
			{
				perror("malloc");
				return (COMMON_SYS_ERR);
			}
#if DEBUG == 1
			dbg_print_configs(c, NONLOGIN_CONF);
#endif
			if (load_nonlogin_configs(msh) == COMMON_SYS_ERR)
				return (COMMON_SYS_ERR);
		}
	}
	return (COMMON_SUCCESS);
}

int	load_login_configs(t_shell *msh)
{
	int		fd;
	int		cnf_i;
	int		gnlerr;
	int		ret_code;
	char	*line;

	cnf_i = 0;
	while (cnf_i < LOGIN_CONFIGS_NUM)
	{
		fd = open(msh->configs.login[cnf_i], O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			return (COMMON_SYS_ERR);
		}
		line = get_next_line(fd, &gnlerr);
		while (line)
		{
			remove_newline(line);
			if (msh->opts.f_verbose)
				printf("%s\n", line);
			shell_engine(NULL, &ret_code);
			line = get_next_line(fd, &gnlerr);
		}
		if (!line && gnlerr)
		{
			print_shell_error(NULL, GNL_ERR_MSG);
			gnl_finish(fd);
			return (COMMON_SYS_ERR);
		}
		if (close(fd) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
		if (cnf_i > 0 && cnf_i < 4)
		{
			cnf_i = 4;
			continue;
		}
		++cnf_i;
	}
	return (COMMON_SUCCESS);
}

int	load_nonlogin_configs(t_shell *msh)
{
	int		fd;
	int		cnf_i;
	int		gnlerr;
	int		ret_code;
	char	*line;

	cnf_i = 0;
	while (cnf_i < NONLOGIN_CONFIGS_NUM)
	{
		fd = open(msh->configs.nonlogin[cnf_i], O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			return (COMMON_SYS_ERR);
		}
		line = get_next_line(fd, &gnlerr);
		while (line)
		{
			remove_newline(line);
			if (msh->opts.f_verbose)
				printf("%s\n", line);
			shell_engine(NULL, &ret_code);
			line = get_next_line(fd, &gnlerr);
		}
		if (!line && gnlerr)
		{
			print_shell_error(NULL, GNL_ERR_MSG);
			gnl_finish(fd);
			return (COMMON_SYS_ERR);
		}
		if (close(fd) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
		++cnf_i;
	}
	return (COMMON_SUCCESS);
}
