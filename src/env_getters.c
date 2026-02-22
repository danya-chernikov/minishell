#include "env.h"

/* Returns variable's value by its name. If a variable
 * with such name does not exist returns NULL */
char	*env_get_val(t_env *env, char *name)
{
	size_t	i;	

	i = 0;
	while (i < env->vars_num)
	{
		if (strings_equal(name, env->vars[i].name))
			return (env->vars[i].value);
		++i;
	}
	return (NULL);
}

/* Returns pointer to the variable by its name. If a
 * variable with such name does not exist returns NULL.
 * We do not care at all here about variable's value */
t_env_var	*env_get_ptr(t_env *env, char *name)
{
	t_env_var	*var;
	size_t		i;

	i = 0;
	while (i < env->vars_num)
	{
		var = &env->vars[i];
		if (strings_equal(name, var->name))
			return (var);
		++i;
	}
	return (NULL);
}
