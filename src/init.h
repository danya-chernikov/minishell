#ifndef INIT_H
# define INIT_H


# include <stdbool.h>

/* minishell options 
 * f_login:		-l, --login;
 * f_verbose:	-v, --verbose;
 * f_bashcompl:	--bash-compliant;
 * f_noprofile:	--noprofile;
 * f_norc:		--norc;
 * f_initfile:	--init-file, --rc-file. */
typedef struct s_options
{
	bool	f_login;
	bool	f_verbose;
	bool	f_bashcompl;
	bool	f_noprofile;
	bool	f_norc;
	bool	f_initfile;
}	t_options;

/* minishell config files
 * etc_prof_path	  - DEF_MINISHL_ETC_PROF_PATH		="/etc/profile";
 *						DEF_BASH_ETC_PROF_PATH			="/etc/profile";
 *
 * home_prof_path	  - DEF_MINISHL_HOME_PROF_PATH		="~/.minishell_profile";
 *					    DEF_BASH_HOME_PROF_PATH			="~/.bash_profile";
 *
 * home_login_path	  - DEF_MINISHL_HOME_LOGIN_PATH		="~/.minishell_login";
 *					  - DEF_BASH_HOME_LOGIN_PATH		="~/.bash_login";
 *
 * home_cmn_prof_path - DEF_MINISHL_HOME_CMN_PROF_PATH	="~/.profile";
 *						DEF_BASH_HOME_CMN_PROF_PATH		="~/.profile";
 * 
 * home_logout_path	  - DEF_MINISHL_HOME_LOGOUT_PATH	="~/.minishell_logout";
 *					  - DEF_BASH_HOME_LOGOUT_PATH		="~/.bash_logout";
 *
 * etc_logout_path	  - DEF_MINISHL_ETC_LOGOUT_PATH
 *						="/etc/minishell.minishell_logout";
 *					  - DEF_BASH_ETC_LOGOUT_PATH
 *						="/etc/bash.bash_logout";
 *
 * etc_hist_path	  - DEF_MINISHL_HOME_HIST_PATH		"~/.minishell_history";
 *						DEF_BASH_HOME_HIST_PATH			="~/.bash_history";
 *
 * etc_rc_path		  - DEF_MINISHL_ETC_RC_PATH
 *						="/etc/minishell.minishellrc";
 *					  - DEF_BASH_ETC_RC_PATH
 *						="/etc/bash.bashrc";
 *
 * home_rc_path		  - DEF_MINISHL_HOME_RC_PATH		="~/.minishellrc";
 *					  - DEF_BASH_HOME_RC_PATH			="~/.bashrc". */
typedef struct s_configs
{
	char	*etc_prof_path;
	char	*home_prof_path;
	char	*home_login_path;
	char	*home_cmn_prof_path;
	char	*home_logout_path;
	char	*etc_logout_path;
	char	*home_hist_path;
	char	*etc_rc_path;
	char	*home_rc_path;
}	t_configs;

/* minishell settings */
typedef struct s_settings
{
	t_options	options;
	t_configs	configs;
}	t_settings;

#endif
