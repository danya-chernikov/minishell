#ifndef ENV_H
# define ENV_H

# include <stdbool.h>
# include <stddef.h>

/* The default path is used when
 * it isn't inherited from the
 * parent or found in any configs */
# define DEF_PATH			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
# define DEF_PS1			"\s-\v\$"
# define DEF_PS2			">"
# define DEF_PS4			"+"

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

/* If type is PARAM, value usually
 * (but not always.. this applies
 * to positional script parameters)
 * is a heap-allocated duplicate of
 * the corresponding argument in argv;
 *
 * If type is ENV, value points to a
 * heap-allocated duplicate of the
 * corresponding value in env (third
 * argument of main function); if it
 * was not inherited, it points to a
 * string determined internally (also
 * heap-allocated);
 *
 * If type is LOCAL, value points to a
 * heap-allocated string determined
 * internally.
 *
 * A variable is considered non-existent
 * when its name is NULL and/or its value
 * is NULL */
typedef struct s_env_var
{
	t_var_type	type;
	bool		f_readonly;
	char		*name;
	char		*value;

}	t_env_var;

/* inh_env - inherited environment from our
 *			 parent (the third argument of
 *			 the main function) */
typedef struct s_env
{	
	size_t		vars_num;
	char		**inh_env;
	t_env_var	*vars;// on heap

}	t_env;

int			env_init(t_env *env);
char		*env_get_val(t_env *env, char *name);
t_env_var	*env_get_ptr(t_env *env, char *name);
void		env_set(t_env *env, char *name, char *value);
void		env_unset(t_env *env, char *name);
void		env_export(t_env *env, char *name);
void		env_print(t_env *env, char *name);

#endif
