#include "shell.h"

#include <stdio.h>
#include <stdlib.h>

// LOGNAME		
void	set_env_logname(t_env *env)
{
	env->vars[SE_LOGNAME].name = ft_strdup("LOGNAME");
	env->vars[SE_LOGNAME].value = ft_strdup(UNKNOWN_VALUE); // "?"
}

/* Sets variables defined by the user
 * in the parent shell session. It may
 * be thought that this causes a memory
 * leak, but in fact env_set() releases
 * `var_name` and/or `var_value`l in case
 * of an internal error */
int	set_rest_env_vars(t_env *env)
{
	int		vi;
	int		res;
	char	*var_name;
	char	*var_value;

	vi = 0;
	// Traverse all the environment
	while (env->inh_env[vi])
	{
		var_name = NULL;
		var_value = NULL;
		res = div2_str_by_delim(env->inh_env[vi], '=', &var_name, &var_value);
		if (res == COMMON_SYS_ERR)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		else if (res == COMMON_FAILURE)
			return (COMMON_FAILURE);
		// Inside this function, a check for the
		// existence of the variable is already
		// implemented. If the variable already
		// exists, the function simply returns
		// NULL and does not add it
		res = env_set(env, var_name, var_value, ENV);
		if (res != COMMON_SUCCESS)
			return (res);
		++vi;
	}
	return (COMMON_SUCCESS);
}
