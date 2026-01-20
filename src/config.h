#ifndef CONFIG_H
#define CONFIG_H

// Where to put it if not here?
# define HOSTNAME_PATH					"/etc/hostname"

/* LOGIN SHELL*/

/* Original bash default configs paths */
# define DEF_BASH_ETC_PROF_PATH			"/etc/profile"
# define DEF_BASH_HOME_PROF_PATH		"~/.bash_profile"
# define DEF_BASH_HOME_LOGIN_PATH		"~/.bash_login"
# define DEF_BASH_HOME_CMN_PROF_PATH	"~/.profile"
# define DEF_BASH_HOME_LOGOUT_PATH		"~/.bash_logout"
# define DEF_BASH_ETC_LOGOUT_PATH		"/etc/bash.bash_logout"

/* minishell default configs paths */
/* `/etc/profile`					 - also called system-wide startup file;
 * `~/.minishell_profile`			 - `~/.bash_profile` analog;
 * `~/.minishell_login`				 - `~/.bash_login` analog;
 * `~/.profile`						 - `~/.profile` analog;
 * `~/.minishell_logout`			 - `~/.bash_logout` analog;
 * `/etc/minishell.minishell_logout` - `/etc/bash.bash_logout` analog;
 * `~/.minishell_history`			 - `~/.bash_history`. */
# define DEF_MSH_ETC_PROF_PATH		"/etc/profile"
# define DEF_MSH_HOME_PROF_PATH		"~/.minishell_profile"
# define DEF_MSH_HOME_LOGIN_PATH	"~/.minishell_login"
# define DEF_MSH_HOME_CMN_PROF_PATH	"~/.profile"
# define DEF_MSH_HOME_LOGOUT_PATH	"~/.minishell_logout"
# define DEF_MSH_ETC_LOGOUT_PATH	"/etc/minishell.minishell_logout"

/* NON-LOGIN SHELL */
/* Original bash default configs paths */
/* `/etc/bash.bashrc`	- also called system-wide initialization file;
 * `~/.bashrc`			- also called personal initialization file. */
# define DEF_BASH_ETC_RC_PATH			"/etc/bash.bashrc"
# define DEF_BASH_HOME_RC_PATH			"~/.bashrc"

/* minishell default configs paths */
/* `/etc/minishell.minishellrc` - `/etc/bash.bashrc` analog;
 * `~/.minishellrc`				- `~/.bashrc` analog. */
# define DEF_MSH_ETC_RC_PATH		"/etc/minishell.minishellrc"
# define DEF_MSH_HOME_RC_PATH		"~/.minishellrc"

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

#endif
