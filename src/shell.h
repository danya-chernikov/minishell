#ifndef SHELL_H
# define SHELL_H

# include "config.h"
# include "history.h"
# include "error.h"
# include "env.h"

# include <stdbool.h>

/* Minishell information */
# define MSH_NAME_SHORT		"msh"
# define MSH_NAME_LONG		"minishell"
# define MSH_VERSION		"1.0-release"
# define MSH_ARCH			"x86_64"
# define MSH_OSTYPE			"linux-gnu"

/* Minishell options */
# define LOGIN_LONG_OPT		"--login"
# define VERBOSE_LONG_OPT	"--verbose"
# define VERSION_LONG_OPT	"--version"
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

/* NONINT_SCRIPT_MODE - non-interactive script executing:
 * In this mode we do NOT read any configs (check this)
 *     ./minishell script.sh
 *     ./script.sh (with shebang inside)
 * NONINT_CMD_MODE - non-interactive shell launch with `-c` option:
 * In this mode we do NOT read any configs (check this)
 *     ./minishell -c "..."
 *     ./minishell -c '...'
 * NONINT_STDIN_MODE - non-interactive shell but accepting commands from STDIN
 * In this mode we do NOT read any configs (check this)
 *     ./minishell -c "..."
 *     ./minishell | wc -l; ls; exit
 *     echo ls | ./minishell | wc -l
 *     ./minishell < commands_file
 * INT_MODE - interactive login or non-login shell:
 * Only in this mode we read configs and support history
 *     ./minishell -l
 *     ./minishell --login
 *     ./minishell */
typedef enum e_shell_mode
{
	NONINT_SCRIPT_MODE,	// ./minishell script.sh arg1 arg2
	NONINT_CMD_MODE,	// ./minishell -c 'command' OR ./minishell -c "command"
	NONINT_STDIN_MODE,	// echo "ls -la" | ./minishell OR ./minishell < commands_file
	INT_MODE,			// ./minishell --login OR ./minishell -l OR ./minishell

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
 *					  (Set to the same value as HISTFILESIZE).
 *
 * c_cmd		- all what goes after -c option in case if present;
 *
 * vars_num		- number of envionment variables we crrently have. */
typedef struct e_shell
{
	t_shell_mode	mode;
	t_options		opts;
	t_configs		configs;
	t_history		history;
	t_env			env;
	int				argc;
	char			**argv;
	char			*prompt_inv;
	char			*script;//on stack
	char			*c_cmd;// on stack

}	t_shell;

int		msh_init(t_shell *msh, int argc, char **argv, char **env);

int		msh_init_param_vars(t_env *env);
void	msh_free_all_vars(t_env *env);

int		msh_set_local_vars(t_env *env, char **argv);
int		msh_set_env_vars(t_env *env);

void	msh_free(t_shell *msh);

#endif
