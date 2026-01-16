#include "env.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int	env_init(t_env *env)
{
	size_t	i;

	env->vars = malloc(MAX_ENV_VARS_NUM * sizeof *env->vars);
	if (!env->vars)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	i = 0;
	while (i < MAX_ENV_VARS_NUM)
	{
		env->vars[i].name = NULL;
		env->vars[i].value = NULL;
		++i;
	}
	return (COMMON_SUCCESS);
}

/* Returns variable's value by its name. If a variable
 * with such name does not exist returns NULL */
char	*env_get_val(t_env *env, char *name)
{
	size_t	i;	

	i = 0;
	while (i < env->vars_num)
	{
		if (!ft_strcmp(env->vars[i].name, name, ft_strlen(name)) &&
			ft_strlen(env->vars[i].name) == ft_strlen(name))
		{
			return (env->vars[i].value);
		}
		++i;
	}
	return (NULL);
}

/* Returns pointer to the variable by its name. If a
 * variable with such name does not exist returns NULL */
t_env_var	*env_get_ptr(t_env *env, char *name)
{
	t_env_var	*var;

	var = env->vars[0];
	while (var)
	{
		if (!ft_strcmp(var->name, name, ft_strlen(name)) &&
			ft_strlen(env->name) == ft_strlen(name))
		{
			return (var);
		}
		++var;
	}
	return (NULL);
}

/* If a variable named `name` already exists,
 * its old value is overwritten with `value`.
 * If it does not exist, a new variable named
 * `name` is created with the value `value` */
int	env_set(t_env *env, char *name, char *value) // Or maybe **value?
{
	t_env_var	*var;

	var = env_get_ptr(env, name);
	if (var)
	{
		if (!var->f_readonly)
		{
			if (var->value)
				free(var->value);
			var->value = value;
		}
		else
			return (COMMON_FAILURE);
	}
	// A variable with this name
	// has not been created yet
	else
	{
		env->vars[env->vars_num - 1].name = name;
		env->vars[env->vars_num - 1].value = value;
	}
	return (COMMON_SUCCESS);
}

void	env_unset(t_env *env, char *name)
{
	(void)name;
}

void	env_export(t_env *env, char *name)
{
	(void)name;
}

void	env_print(t_env *env, char *name)
{
	(void)name;
}
