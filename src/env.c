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
		if (!ft_strncmp(env->vars[i].name, name, ft_strlen(name)) &&
			ft_strlen(env->vars[i].name) == ft_strlen(name))
		{
			return (env->vars[i].value);
		}
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

	var = &env->vars[0];
	while (var)
	{
		if (!ft_strncmp(var->name, name, ft_strlen(name)) &&
			ft_strlen(var->name) == ft_strlen(name))
		{
			return (var);
		}
		++var;
	}
	return (NULL);
}

bool	env_exist(t_env *env, char *name)
{
	return (env_get_ptr(env, name));
}

/* If a variable named `name` already exists,
 * its old value is overwritten with `value`.
 * If it does not exist, a new variable named
 * If a variable named `name` has the read-only
 * flag set returns an error. Both `name` and
 * `value` must live on heap! */
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

/* Unsets the variable named `name`. If no
 * variable with this name exists, or if it
 * has the read-only flag set, the function
 * returns an error */
int	env_unset(t_env *env, char *name)
{
	t_env_var	*var;

	var = env_get_ptr(env, name);
	if (var)
	{
		if (!var->f_readonly)
		{
			if (var->value)
				free(var->value);
			var->value = NULL;
		}
		else
			return (COMMON_FAILURE);
	} // Otherwise the variable did not exist
	else
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}

/* Exports the variable named name. If no
 * variable with this name exists, an error
 * is returned. Parameter variables cannot
 * be exported. Only local variables created
 * during the session can be exported.
 * Technically, it is possible to export
 * 'special' local variables defined by our
 * shell, such as BASHPID, BASH_SUBSHELL,
 * BASH_VERSION, ... PPID, UID, and EUID.
 * However, even if these variables are
 * exported, they will be overwritten in
 * child shells immediately. Additionally,
 * descendant shells will read their
 * configuration files, and if a configuration
 * defines a 'special' local variable (except
 * PPID, UID, and EUID, which are always
 * determined internally), its value will be
 * changed */
int	env_export(t_env *env, char *name)
{
	t_env_var	*var;

	var = env_get_ptr(env, name);
	if (var)
	{
		if (var->type == LOCAL)
			var->type = ENV;
	}
	else
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}

/* Prints the value of the varible named `name`.
 * If this variable does not exist, print a newline.
 * So it's quite similar to `echo $VAR` behaviour */
void	env_print_value(t_env *env, char *name)
{
	t_env_var	*var;

	var = env_get_ptr(env, name);
	if (var)
		printf("%s", var->value);
	printf("\n");
}

/* Basically, this is an implementation
 * of the `printenv` and `env` built-ins.
 * It prints only environment variables
 * that are inherited by our child
 * processes */
void	env_print_env(t_env *env)
{
	(void)env;	
}

/* Implementation of the `set` built-in
 * without any arguments. Prints both
 * all local variables and all
 * environment variables */
void	env_print_all(t_env *env)
{
	(void)env;	
}

/* I honestly have no idea
 * why bash does not provide
 * a built-in command to print
 * only the local variables
 * we create during a session.
 * Well.. this function does
 * exactly that wuw */
void	env_print_locals(t_env *env)
{
	(void)env;	
}
