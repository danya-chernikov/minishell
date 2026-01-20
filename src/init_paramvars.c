#include "shell.h"

#include <stdio.h>

static void	prelim_vars_init(t_env *env);
static void	set_var_names(t_env *env);
static int	check_mem_errors(t_env *env);

/* For each environment variable name
 * and value, we will always allocate
 * memory on the heap, so it will be
 * easier to free everything later
 * in one go */
int	msh_init_param_vars(t_env *env)
{
	int		res;
	pid_t	pid;

	prelim_vars_init(env);
	set_var_names(env);
	env->vars_num = PARAM_VARS_NUM;
	// Check for memory errors
	if (check_mem_errors(env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	// Why not to set $$, $# and $0 immediately...
	// Set $$
	res = ft_getpid(&pid);
	if (res == -1)
	{
		print_shell_error("ft_getpid()", GETPID_ERR_MSG);
		return (COMMON_SYS_ERR);
	}
	env->vars[PV_PID].value = ft_itoa((int)pid);
	// Set $#
	env->vars[PV_ARGNUM].value = ft_strdup("0");
	return (COMMON_SUCCESS);
}

static void	prelim_vars_init(t_env *env)
{
	t_paramvar	vi;

	vi = PV_HOME;
	// Init ~, $?
	while (vi < PV_PID)
	{
		env->vars[vi].type = PARAM;
		env->vars[vi].f_inherit = false;
		env->vars[vi].value = NULL;
		++vi;
	}
	// Init all the rest variables
	while (vi < PARAM_VARS_NUM)
	{
		env->vars[vi].type = PARAM;
		env->vars[vi].f_readonly = true;
		env->vars[vi].f_inherit = false;
		env->vars[vi].value = NULL; // Do we need this?
		++vi;
	}
}

static void	set_var_names(t_env *env)
{
	env->vars[PV_HOME].name		= ft_strdup("~"); // may be changed!
	env->vars[PV_RETCODE].name	= ft_strdup("$?");// may be changed!
	env->vars[PV_PID].name		= ft_strdup("$$");
	env->vars[PV_ARGNUM].name	= ft_strdup("$#");
	env->vars[PV_ALLARGS].name	= ft_strdup("$*");
	env->vars[PV_ARGV0].name	= ft_strdup("$0");
	env->vars[PV_ARGV1].name	= ft_strdup("$1");
	env->vars[PV_ARGV2].name	= ft_strdup("$2");
	env->vars[PV_ARGV3].name	= ft_strdup("$3");
	env->vars[PV_ARGV4].name	= ft_strdup("$4");
	env->vars[PV_ARGV5].name	= ft_strdup("$5");
	env->vars[PV_ARGV6].name	= ft_strdup("$6");
	env->vars[PV_ARGV7].name	= ft_strdup("$7");
	env->vars[PV_ARGV8].name	= ft_strdup("$8");
	env->vars[PV_ARGV9].name	= ft_strdup("$9");
}

static int	check_mem_errors(t_env *env)
{
	t_paramvar	vi;

	vi = PV_HOME;
	while (vi < PARAM_VARS_NUM)
	{
		if (!env->vars[vi].name)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++vi;
	}
	return (COMMON_SUCCESS);
}
