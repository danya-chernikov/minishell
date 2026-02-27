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

# define MUST_EXIT			-7
# define PROMPT_INV_LEN		512	// Maximum length of user's prompt invitation string
# define MAX_SUBDOMAIN_LEN	64

typedef struct s_parser_data	t_parser_data;

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

typedef enum e_shell_mode
{
	NONINT_SCRIPT_MODE,	// ./minishell script.sh arg1 arg2
	NONINT_CMD_MODE,	// ./minishell -c 'command' OR ./minishell -c "command"
	NONINT_STDIN_MODE,	// echo "ls -la" | ./minishell OR ./minishell < commands_file
	INT_MODE,			// ./minishell --login OR ./minishell -l OR ./minishell

}	t_shell_mode;

typedef struct s_shell
{
	t_parser_data	*pd;
	t_shell_mode	mode;
	t_options		opts;
	t_configs		configs;
	t_history		history;
	t_env			env;
	int				argc;
	char			**argv;
	char			*prompt_inv;	// on heap
	char			*script;		// on stack
	char			*c_cmd;			// on stack

}	t_shell;

/* shell.c */
int		msh_init(t_shell *msh, int argc, char **argv, char **env);
void	msh_free(t_shell *msh);

/* shell_launcher.c */
int		msh_launch(t_shell *msh, int *ret_code);
int		launch_cmd(t_shell *msh, int *ret_code);
int		launch_stdin_cmd(t_shell *msh, int *ret_code);

/* shell_launch_int.c */
int		launch_int_session(t_shell *msh, int *ret_code);

/* shell_launch_script.c */
int		launch_script(t_shell *msh, int *ret_code);

/* aux_shell.c */
int		gen_prompt_inv(t_shell *msh);

/* aux_shell2.c */
void	msh_update_retcode(t_shell *msh, int status);

/* shell_configs.c */
int		msh_load_configs(t_shell *msh);
int		load_login_configs(t_shell *msh);
int		load_nonlogin_configs(t_shell *msh);

/* shell_history.c */
int		msh_load_history(t_shell *msh);

/* init_paramvars.c  */
int		msh_init_param_vars(t_env *env);

/* LOCAL VARIABLES */
/* init_localvars.c */
int		msh_set_local_vars(t_env *env, char **argv);

/* init_localvars2.c */
int		set_local_ppid(t_env *env);
int		set_local_uid(t_env *env);
int		set_local_euid(t_env *env);
int		set_local_mshpid(t_env *env);
int		set_local_mshsubsh(t_env *env);

/* init_localvars3.c */
int		set_local_mshversion(t_env *env);
int		set_local_histfilesize(t_env *env);
int		set_local_histfile(t_env *env);
int		set_local_histsize(t_env *env);
int		set_local_msh(t_env *env, char **argv);

/* init_localvars4.c */
int		set_local_hosttype(t_env *env);
char	*get_hostname(void);
int		set_local_hostname(t_env *env);
int		set_local_ostype(t_env *env);
int		set_local_hostmach(t_env *env);

/* init_localvars5.c */
int		set_local_ps1(t_env *env);
int		set_local_ps2(t_env *env);
int		set_local_ps4(t_env *env);

/* ENVIRONMENT VARIABLES */
/* init_envars.c */
int		msh_set_env_vars(t_env *env);

/* init_envars2.c */
void	set_env_path(t_env *env);
void	set_env_shelevel(t_env *env);
int		set_env_pwd(t_env *env);
void	set_env_oldpwd(t_env *env);
int		set_env_pwd_user_data(t_env *env);

/* init_envars3.c */
void	set_env_logname(t_env *env);
int		set_rest_env_vars(t_env *env);

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

#endif
