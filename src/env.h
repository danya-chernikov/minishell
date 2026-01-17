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
 *			   15. PPID					(can't be changed/deleted)
 *			   16. UID					(can't be changed/deleted)
 *			   17. EUID					(can't be changed/deleted)
 *             18. MSHPID				(can't be changed/deleted)
 *			   19. MSH_SUBSHELL			(has default)
 *			   20. MSH_VERSION			(has default)
 *			   21. HISTFILESIZE			(has default)
 *			   22. HISTFILE				(has default)
 *			   23. HISTSIZE				(has default)
 *			   24. MSH					(unknown)
 *			   25. HOSTNAME				(has default)
 *			   26. HOSTTYPE				(has default)
 *			   27. OSTYPE				(has default)
 *			   28. MACHTYPE				(has default)
 *			   29. PS1					(has default)
 *			   30. PS2					(has default)
 *			   31. PS4					(has default)
 *
 *         MINISHELLPID is an analogue of BASHPID. It is a special variable
 *         whose value is calculated at the moment a (sub)shell expands it.
 *
 *         MINISHELL_SUBSHELL is an analogue of BASH_SUBSHELL. It is similar to
 *         SHLVL, but it is local and specific to subshells. Using this variable,
 *         we can determine whether we are running inside a subshell. Its value
 *         is incremented each time a new shell is started.
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
 *             29. PATH					(has default)
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
# define SLOCAL_VARS_NUM	17
# define SENV_VARS_NUM		8
# define SCRIPT_ARGS_NUM	9
# define MAX_TOTAL_VARS_NUM	8192

/* Do not change the order of these
 * definitions! Otherwise, you'll
 * break initialization of parameter
 * variables (see msh_init_param_vars()
 * function for reference) */
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

/* Special local variables are set
 * by the shell. They can all be
 * changed or deleted (except MSHPID),
 * but they cannot be exported.
 * Do not change order of these
 * definitions! Otherwise, you'll
 * break initialization of special local
 * variables (see msh_set_local_vars()
 * function for reference) */
typedef enum e_special_localvar
{
	SL_MSHPID=16,	// MSHPID (can)
	SL_MSHSUBSH,	// MSH_SUBSHELL
	SL_MSHVER,		// MSH_VERSION
	SL_HFSIZE,		// HISTFILESIZE
	SL_HFILE,		// HISTFILE
	SL_HSIZE,		// HISTSIZE
	SL_MSH,			// MSH
	SL_HOSTNAME,	// HOSTNAME
	SL_HOSTTYPE,	// (has default)
	SL_OSTYPE,		// (has default)
	SL_MACHTYPE,	// (has default)
	SL_PS1,			// PS1
	SL_PS2,			// PS2
	SL_PS4,			// PS4
	SL_PPID,		// PPID 
	SL_UID,			// UID
	SL_EUID			// EUID

}	t_slocalvar;

/* These are important environment variables
 * that affect the functioning of our shell.
 * almoost*/
typedef enum e_special_envar
{
	SE_PATH=32,
	SE_SHLVL,
	SE_OLDPWD,
	SE_PWD,
	SE_HOME,
	SE_SHELL,
	SE_USER,
	SE_LOGNAME
}	t_special_envar;

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
