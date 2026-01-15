#ifndef ENV_H
# define ENV_H

# include <stdbool.h>

/* The default path is used when
 * it isn't inherited from the
 * parent or found in any configs */
# define DEF_PATH			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
# define PARAM_VARS_NUM		15
# define SCRIPT_ARGS_NUM	9
# define LOCAL_VARS_NUM		14
# define MAX_ENV_VARS_NUM	1024

typedef enum e_paramvar
{
	PV_HOME = 0,// ~
	PV_RETCODE,	// $?
	PV_PID,		// $$
	PV_ARGNUM,	// $#
	PV_ALLARGS,	// $* Accessible only in scripts!
	PV_ARGV0,	// if Equals '-minishell' our shell was run as login-shell
	PV_ARGV1,	// Only in scripts	
	PV_ARGV2,	// ...
	PV_ARGV3,	// ...
	PV_ARGV4,	// ...
	PV_ARGV5,	// ...
	PV_ARGV6,	// ...
	PV_ARGV7,	// ...
	PV_ARGV8,	// ...
	PV_ARGV9,	// Only in scripts
	
}	t_paramvar;

typedef enum e_var_type
{
	LOCAL,		// MINISHELL_VERSION, HISTFILESIZE, HISTFILE, HISTSIZE, UID, EUID and etc.
	ENV,		// PATH, SHLVL, OLDPW, PWD, HOME, SHELL, USER and etc.
	PARAM		// ~, $?, $$, $#, $*, $0, $1, $2, $3, $4, $5, $6, $7, $8, $9
}	t_var_type;

/* If `type` is PARAM, `value` points to
 * the corresponding argument in `argv`;
 *
 * If `type` is INHERIT, `value` points
 * to the corresponding value in `env`
 * or if it wasn't inherited it points
 * to the string we determine ourself;
 *
 * If `type` is LOCAL, `value` points to
 * the string we determine ourselfs.
 * */
typedef struct s_env_var
{
	t_var_type	type;
	bool		f_readonly;
	char		*name;		// We'll just not have any limit here!
	char		*value;		// Here as well, no limit

}	t_env_var;

int		init_param_vars(t_env_var *vars);

int		set_env_vars(t_env_var *vars);
int		set_local_vars(t_env_var *vars);

void	print_env();
void	set_env();
void	unset_env();
void	export_env();

void	free_param_vars(t_env_var *vars);

#endif
