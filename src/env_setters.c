#include "env.h"
#include "error.h"

#include <stdlib.h>

/* Service functions for env_set() */
static int	set_existing_var(t_env_var *var, char *name, char *value);
static int	check_bounds(t_env *env, char *name, char *value);
static void	create_new_var(t_env *env, char *name, char *val, t_var_type type);

/* If a variable named `name` already exists, its old value is
 * overwritten with `value`. `value` and `name`  must always be
 * heap-allocated. If the variable named `name` has the read-only
 * flag set, the function returns an error. If env_set() fails to
 * create a new variable or assign a new value to exiting variable,
 * it frees both `name` and `value` */
int	env_set(t_env *env, char *name, char *value, t_var_type type)
{
	t_env_var	*var;

	var = env_get_ptr(env, name);
	// Variables already exists
	if (var)
	{
		if (!set_existing_var(var, name, value))
			return (COMMON_FAILURE);
	}
	// A variable with this name has not been created yet
	else
	{
		if (!check_bounds(env, name, value))
			return (COMMON_FAILURE);
		create_new_var(env, name, value, type);
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

static int	set_existing_var(t_env_var *var, char *name, char *value)
{
	if (!var->f_readonly)
	{
		if (ft_strlen(value) >= MAX_ENV_VAL_LEN)
		{
			print_shell_error(NULL, MAX_ENV_VAL_ERR_MSG);
			free(value); // We just leave variable's value unchanged
			return (COMMON_FAILURE);
		}
		if (var->value)
			free(var->value);
		var->value = value;
		free(name); // `name` was already allocated!
	}
	else // Variable exists but is read-only
	{
		print_shell_error(name, READONLY_VAR_ERR_MSG);
		free(name);
		free(value);
		return (COMMON_FAILURE);
	}
	return (COMMON_SUCCESS);
}

static int	check_bounds(t_env *env, char *name, char *value)
{
	if (env->vars_num == MAX_TOTAL_VARS_NUM)
	{
		free(name);
		free(value);
		// Variable is too long.. so why to print it xd
		print_shell_error(NULL, MAX_ENV_NUM_ERR_MSG);
		return (COMMON_FAILURE);
	}
	if (ft_strlen(name) >= MAX_ENV_NAME_LEN)
	{
		print_shell_error(NULL, MAX_ENV_NAME_ERR_MSG);
		free(name);
		free(value);
		return (COMMON_FAILURE);
	}
	if (ft_strlen(value) >= MAX_ENV_VAL_LEN)
	{
		print_shell_error(NULL, MAX_ENV_VAL_ERR_MSG);
		free(name);
		free(value);
		return (COMMON_FAILURE);
	}
	return (COMMON_SUCCESS);
}

static void	create_new_var(t_env *env, char *name, char *value, t_var_type type)
{
	env->vars[env->vars_num].type = type;
	env->vars[env->vars_num].f_readonly = false;
	if (type == ENV)
		env->vars[env->vars_num].f_inherit = true;
	else
		env->vars[env->vars_num].f_inherit = false;
	env->vars[env->vars_num].name = name;
	env->vars[env->vars_num].value = value;
	++env->vars_num;
}
