/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:52:38 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 00:52:38 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdbool.h>
# include <stddef.h>

# include "aux_common.h"

/* The default path is used when it isn't inherited
 * from the parent or found in any configs.
 *
 * In reality, of course, the values of LANG and
 * LANGUAGE are defined differently, for example
 * by reading configs of smth.. Here at 42 Urduliz,
 * it is currently read from `/etc/default/locale`
 * if it was not inherited. But let's implement
 * this later */
# define DEF_PS1			"\\s-\\v\\$"
# define DEF_PS2			"> "
# define DEF_PS4			"+ "
# define DEF_LANGUAGE		"en"
# define DEF_LANG			"en_US.UTF-8"
# define UNKNOWN_VALUE		"?"

# define PARAM_VARS_NUM		15
# define SLOCAL_VARS_NUM	17
# define SENV_VARS_NUM		8
# define SCRIPT_ARGS_NUM	9
# define MAX_TOTAL_VARS_NUM	8192

# define MAX_ENV_NAME_LEN	4096
# define MAX_ENV_VAL_LEN	4096

/* Do not change the order of these
 * definitions! Otherwise, you'll
 * break initialization of parameter
 * variables (see msh_init_param_vars()
 * function for reference) */
typedef enum e_paramvar
{
	PV_HOME = 0,
	PV_RETCODE,
	PV_PID,
	PV_ARGNUM,
	PV_ALLARGS,
	PV_ARGV0,
	PV_ARGV1,
	PV_ARGV2,
	PV_ARGV3,
	PV_ARGV4,
	PV_ARGV5,
	PV_ARGV6,
	PV_ARGV7,
	PV_ARGV8,
	PV_ARGV9 = 14
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
	SL_PPID = 15,
	SL_UID,
	SL_EUID,
	SL_MSHPID,
	SL_MSHSUBSH,
	SL_MSHVER,
	SL_HFSIZE,
	SL_HFILE,
	SL_HSIZE,
	SL_MSH,
	SL_HOSTNAME,
	SL_HOSTTYPE,
	SL_OSTYPE,
	SL_MACHTYPE,
	SL_PS1,
	SL_PS2,
	SL_PS4 = 31
}	t_slocalvar;

/* These are important environment variables
 * that affect the functioning of our shell.
 * almoost*/
typedef enum e_special_envar
{
	SE_PATH = 32,
	SE_SHLVL,
	SE_PWD,
	SE_OLDPWD,
	SE_HOME,
	SE_SHELL,
	SE_USER,
	SE_LOGNAME = 39
}	t_senvar;

typedef enum e_var_type
{
	LOCAL,
	ENV,
	PARAM
}	t_var_type;

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
	t_env_var	*vars;		// on heap
}	t_env;

/* env.c */
int			env_init(t_env *env, char **inh_env);
void		env_free(t_env *env);
char		*env_get_def_path(void);

/* env_setters.c */
int			env_set(t_env *env, char *name, char *value, t_var_type type);
int			env_unset(t_env *env, char *name);

/* env_getters.c */
char		*env_get_val(t_env *env, char *name);
t_env_var	*env_get_ptr(t_env *env, char *name);

/* env_printers.c */
void		env_print_value(t_env *env, char *name);
void		env_print_env(t_env *env);
void		env_print_all(t_env *env);
void		env_print_locals(t_env *env);

/* env_service.c */
bool		env_exist(t_env *env, char *name);
int			env_export(t_env *env, char *name);
size_t		env_count_exported_vars(t_env *env);
size_t		env_count_all_vars(t_env *env);

/* env_exec.c */
int			env_apply_as_local(t_env *dst, t_env *src);
int			env_apply_as_env(t_env *dst, t_env *src);

#endif
