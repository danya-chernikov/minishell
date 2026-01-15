#include "env.h"

/* For each environment variable name and
 * value, we will always allocate memory
 * on the heap, so it will be easier to
 * free everything later in one go */
void	init_param_vars(t_env_var *vars)
{
	size_t	i;
	int		res;
	pid_t	pid;

	i = 0;
	// Init ~, $? and $$
	while (i < 2)
	{
		vars[i].type = PARAM;
		vars[i].value = NULL;
		++i;
	}
	i = 2;
	while (i < PARAMVARS_NUM)
	{
		vars[i].type = PARAM;
		vars[i].f_readonly = true;
		vars[i].value = NULL;
		++i;
	}
	vars[PV_HOME].name		= ft_strdup("~"); // may be changed!
	vars[PV_RETCODE].name	= ft_strdup("$?");// may be changed!
	vars[PV_PID].name		= ft_strdup("$$");
	vars[PV_ARGNUM].name	= ft_strdup("$#");
	vars[PV_ALLARGS].name	= ft_strdup("$*");
	vars[PV_ARGV0].name		= ft_strdup("$0");
	vars[PV_ARGV1].name		= ft_strdup("$1");
	vars[PV_ARGV2].name		= ft_strdup("$2");
	vars[PV_ARGV3].name		= ft_strdup("$3");
	vars[PV_ARGV4].name		= ft_strdup("$4");
	vars[PV_ARGV5].name		= ft_strdup("$5");
	vars[PV_ARGV6].name		= ft_strdup("$6");
	vars[PV_ARGV7].name		= ft_strdup("$7");
	vars[PV_ARGV8].name		= ft_strdup("$8");
	vars[PV_ARGV9].name		= ft_strdup("$9");

	// Check for memory errors
	i = 0;
	while (i < PARAMVARS_NUM)
	{
		if (!vars[i].name)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++i;
	}

	// Why immediately not to set $$ and $#...
	// Set $$
	res = ft_getpid(&pid);
	if (res == -1)
	{
		print_shell_error("ft_getpid()", GETPID_ERR_MSG);
		return (COMMON_SYS_ERR);
	}
	vars[PV_PID].value = ft_itoa((int)pid);
	// Set $#
	vars[PV_ARGNUM].value = ft_strdup("0");

	return (COMMON_SUCCESS);
}

int		set_env_vars(t_env_var *vars)
{

}

int		set_local_vars(t_env_var *vars)
{

}


void	free_param_vars(t_env_var *vars)
{
	size_t	i;

	i = 0;
	while (i < PARAMVARS_NUM)
	{
		if (vars[i].name)
			free(vars[i].name);
		if (vars[i].value)
			free(vars[i].value);
		++i;
	}
}
