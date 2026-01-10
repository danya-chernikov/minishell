#ifndef SHELL_H
# define SHELL_H

# include "init.h"
# include "vector.h"

# define MINISHELL_VERSION	"1.0-release"

/* The default path is used when
 * it isn't inherited from the
 * parent or found in any configs */
# define DEF_PATH			"/usr/local/sbin:"
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
# define DEF_MINISHL_ETC_PROF_PATH		"/etc/profile"
# define DEF_MINISHL_HOME_PROF_PATH		"~/.minishell_profile"
# define DEF_MINISHL_HOME_LOGIN_PATH	"~/.minishell_login"
# define DEF_MINISHL_HOME_CMN_PROF_PATH	"~/.profile"
# define DEF_MINISHL_HOME_LOGOUT_PATH	"~/.minishell_logout"
# define DEF_MINISHL_ETC_LOGOUT_PATH	"/etc/minishell.minishell_logout"
# define DEF_MINISHL_HOME_HIST_PATH		"~/.minishell_history"

/* NON-LOGIN SHELL */
/* Original bash default configs paths */
/* `/etc/bash.bashrc`	- also called system-wide initialization file;
 * `~/.bashrc`			- also called personal initialization file. */
# define DEF_BASH_ETC_RC_PATH			"/etc/bash.bashrc"
# define DEF_BASH_HOME_RC_PATH			"~/.bashrc"

/* minishell default configs paths */
/* `/etc/minishell.minishellrc` - `/etc/bash.bashrc` analog;
 * `~/.minishellrc`				- `~/.bashrc` analog. */
# define DEF_MINISHL_ETC_RC_PATH		"/etc/minishell.minishellrc"
# define DEF_MINISHL_HOME_RC_PATH		"~/.minishellrc"

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
	NONINT_SCRIPT,
	NONINT_CMD,
	INT_LOG,
	INT_NONLOG
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

typedef struct e_hist_cmd
{
	t_vector	cmd;
	t_hist_type	type;
};

/* Special shell parameters */
struct e_shell_params
{

} t_shell_params;

#endif
