#ifndef SHELL_H
# define SHELL_H

# include "vector.h"

# include <stdbool.h>

# define MSH_VERSION	"1.0-release" 
# define MSH_ARCH		"x86_64"
# define MSH_OSTYPE		"linux-gnu"

/* The default path is used when
 * it isn't inherited from the
 * parent or found in any configs */
# define DEF_PATH		"/usr/local/sbin:"
	"/usr/local/bin:"
	"/usr/sbin:"
	"/usr/bin:"
	"/sbin:/bin"
/* When unsetting both HISTSIZE and HISTFILESIZE,
 * they revert to their default values and history
 * continues to work. However, when HISTFILE is
 * unset, writing history to the file in the current
 * shell stops working. In a child shell, the value
 * of HISTFILE is reset to the default, and history
 * is written without any problems */
# define DEF_HISTSIZE		500
# define DEF_HISTFILESIZE	500

/* If executed with the `--bash-compliant`
 * option minishell will read bash configs */

/* LOGIN SHELL*/

/* Original bash default configs paths */
# define DEF_BASH_ETC_PROF_PATH			"/etc/profile"
# define DEF_BASH_HOME_PROF_PATH		"~/.bash_profile"
# define DEF_BASH_HOME_LOGIN_PATH		"~/.bash_login"
# define DEF_BASH_HOME_CMN_PROF_PATH	"~/.profile"
# define DEF_BASH_HOME_LOGOUT_PATH		"~/.bash_logout"
# define DEF_BASH_ETC_LOGOUT_PATH		"/etc/bash.bash_logout"
# define DEF_BASH_HOME_HIST_PATH		"~/.bash_history"

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
# define DEF_MSH_HOME_HIST_PATH		"~/.minishell_history"

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

/* minishell options */
# define LOGIN_LONG_OPT		"--login"
# define VERBOSE_LONG_OPT	"--verbose"
# define NORC_LONG_OPT		"--norc"
# define HELP_LONG_OPT		"--help"
# define LOGIN_SHORT_OPT	"-l"
# define VERBOSE_SHORT_OPT	"-v"
# define C_SHORT_OPT		"-c"

/* minishell options 
 * f_login:		-l, --login;
 * f_verbose:	-v, --verbose;
 * f_norc:		--norc;
 * f_c			-c */
typedef struct s_options
{
	bool	f_login;
	bool	f_verbose;
	bool	f_norc;
	bool	f_c;
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

/* NONINT_SCRIPT - non-interactive script executing:
 *     bash script.sh
 *     ./script.sh (with shebang inside)
 * NONINT_CMD - non-interactive shell launch with `-c` option:
 *     bash -c "..."
 *     bash -c '...'
 * INT_LOG - interactive login shell:
 *     bash -l
 *     bash --login
 * INT_NONLOG - interactive non-login shell:
 *     bash */
typedef enum e_shell_mode
{
	NONINT_SCRIPT,	// bash script.sh arg1 arg2
	NONINT_CMD,		// bash -c 'command' | bash -c "command"
	INT_LOG,		// bash --login | bash -l
	INT_NONLOG		// bash
}	t_shell_mode;

/* Type of the history command.
 * Determines whether this
 * command was read from the
 * history file (in which case
 * it should not be written to
 * the history file again) or
 * entered during the current
 * session (in which case it
 * should be written to the
 * history file, since it is
 * a new command) */
typedef enum e_hist_type
{
	FROM_FILE,
	FROM_SESSION
}	t_hist_type;

/* line_num - number of line this command has
 *			  in history file. Commands from
 *			  current session have it as 0 */
typedef struct e_hist_cmd
{
	size_t		line_num;
	t_vector	cmd;
	t_hist_type	type;
}	t_hist_cmd;

/* histsize		- analogue of HISTSIZE.
 *
 *				      If set to -1, there is no limit on how many commands
 *			          shown when the `history` built-in command is launched.
 *			          (Set to -1 if HISTSIZE is negative, non-numeric or
 *					  does not exist);
 *
 *					  If set to 0, the shell does not show any history. In
 *					  this case, arrow keys and Ctrl+N/Ctrl+P do not work.
 *					  (Set to 0 if HISTSIZE is 0);
 *
 *					  If set to a positive value `n` the shell shows the
 *					  first `n` commands from the current session history.
 *					  (Set to the same value as HISTSIZE).
 *
 * histfilesize - analogue of HISTFILESIZE.
 *
 *					  If set to -1 there is no limit on how many commands
 *					  the current session history are written to the
 *					  history file.
 *					  (Set to -1, if HISTFILESIZE is negative, non-numeric
 *					  or does not exist);
 *
 *					  If set to 0, the history file is immediately truncated
 *					  to zero length (all contents are removed), and nothing
 *					  is written on exit.
 *					  (Set to 0 if HISTFILESIZE is 0);
 *
 *					  If set to a positive value `n`, only the last `n`
 *					  commands from the current session are written to
 *					  the history file on exit.
 *					  (Set to the same value as HISTFILESIZE). */
typedef struct e_shell
{
	t_shell_mode	mode;
	t_options		opts;
	t_configs		configs;
	t_env_var		vars;
	char			**argv;
	char			**env;
	int				histsize;
	int				histfilesize;
	char			*script_path;
}	t_shell;

/* cmds		- commands to execute inside shell;
 * posargv	- positional arguments;
 * pos_argc - number of positional arguemnts;
 * settings	- shell settings;
 * env		- inherited environment.
 *
 * NONINT_SCRIPT (We want our shell execute a script)
 *     cmd=NULL;
 *     mode=NONINT_SCRIPT;
 * NONINT_CMD
 *     script_path=NULL;
 *     mode=NONINT_CMD;
 * INT_LOG
 *     cmds=NULL;
 *     script_path=NULL;
 *     pos_argv=NULL;
 *     pos_argc=0;
 *     mode=INT_LOG;
 * INT_NONLOG
 *     cmds=NULL;
 *     script_path=NULL;
 *     pos_argv=NULL;
 *     pos_argc=0;
 *     mode=INT_NONLOG.
 * */

void	msh_init(t_shell *msh);
void	print_help(void);

#endif
