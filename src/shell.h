#ifndef SHELL_H
# define SHELL_H

# include "vector.h"
# include "config.h"
# include "history.h"
# include "env.h"

# include <stdbool.h>

/* If returns from a function marks its
 * caller should invoke exit(EXIT_SUCCESS) */
# define EXIT_SUCCESS_RET	2

/* Minishell information */
# define MSH_VERSION	"1.0-release" 
# define MSH_ARCH		"x86_64"
# define MSH_OSTYPE		"linux-gnu"

/* Minishell options */
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
	NONINT_SCRIPT,	// ./minishell script.sh arg1 arg2
	NONINT_CMD,		// ./minishell -c 'command' OR ./minishell -c "command"
	NONINT_STDIN,	// echo "ls -la" | ./minishell OR ./minishell < commands_file
	INT_LOG,		// ./minishell --login OR ./minishell -l
	INT_NONLOG		// ./minishell
}	t_shell_mode;

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
	t_env_var		*vars;
	t_hist_cmd		*history;
	int				argc;
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

int		msh_init(t_shell *msh, int argc, char **argv, char **env);
void	msh_free(t_shell *msh);

#endif
