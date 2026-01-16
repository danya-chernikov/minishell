#ifndef ENV_H
# define ENV_H

# include <stdbool.h>

/* Each variable's number here corresponds to its index in the `vars`
 * array of the `s_shell` structure. In our minishell, we have the
 * following kinds of variables:
 *
 *     SPECIAL PARAMETER VARIABLES:
 *         Shell parameter variables with special behavior.
 *         Netiher of these variables can be changed or deleted.
 *		       0.  ~
 *             1.  $?
 *             2.  $$
 *             3.  $#
 *             4.  $*
 *             5.  $0
 *             6.  $1
 *             7.  $2
 *             8.  $3
 *             9.  $4
 *             10. $5
 *             11. $6
 *             12. $7
 *             13. $8
 *             14. $9
 *
 *     SO-CALLED 'SPECIAL' LOCAL VARIABLES (`f_inherit` flag is cleared)
 *         Local variables that have a critical or near-critical role
 *         in the shell's operation. Some of them cannot be modified
 *         or deleted (`f_readonly` flag is set), while others can
 *         (`f_readonly` flag is cleared);
 *         The complete list of them:
 *             15. MINISHELLPID			(can't be changed/deleted)
 *			   16. MINISHELL_SUBSHELL	(has default)
 *			   17. MINISHELL_VERSION	(has default)
 *			   18. HISTFILESIZE			(has default)
 *			   19. HISTFILE				(has default)
 *			   20. HISTSIZE				(has default)
 *			   21. MINISHELL
 *			   22. HOSTNAME
 *			   23. PS1					(has default)
 *			   24. PS2					(has default)
 *			   25. PS4					(has default)
 *			   26. PPID					(can't be changed/deleted)
 *			   27. UID					(can't be changed/deleted)
 *			   28. EUID					(can't be changed/deleted)
 *
 *     CLASSICAL ENVIRONMENT VARIABLES (`f_inherit` flag is set)
 *         Variables inherited by child processes. Nothing special here.
 *         There are variables that were inherited. We can change the
 *         value of any of them. However, among these variables there
 *         are some special beasts that directly affect the shell's
 *         workflow and behavior. Changing or deleting them may somewhat
 *         'ruin' the current shell session as well as all its ancestors.
 *         Some of these variables are even managed dynamically by the
 *         shell itself, for example $SHLVL
 *         The complete list of them:
 *             29. PATH
 *             30. SHLVL
 *             31. OLDPWD
 *             32. PWD
 *             33. HOME
 *             34. SHELL
 *             35. USER
 *             36. LOGNAME
 *
 *         So yeah.. the first 37 indices in vars are reserved for easier
 *         access. After that, all newly added local or environment variables
 *         will have mixed indices
 *
 *     USER'S LOCAL VARIABLES 
 *         Created by the user. These variables may be exported
 *         and therefore inherited by child processes;
 *         Examples: 
 *             BLA='keke'
 *             LOL='azaza'*
 *
 * */

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
 * is NULL.
 *
 *     f_inherit - marks if a variable
 *				   may be exported or not */

typedef struct s_env_var
{
	t_var_type	type;
	bool		f_readonly;
	bool		f_inherit; 
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
bool		env_exist(t_env *env, char *name);
int			env_set(t_env *env, char *name, char *value);
int			env_unset(t_env *env, char *name);
int			env_export(t_env *env, char *name);
void		env_print_value(t_env *env, char *name);
void		env_print_env(t_env *env);
void		env_print_all(t_env *env);
void		env_print_locals(t_env *env);

#endif
