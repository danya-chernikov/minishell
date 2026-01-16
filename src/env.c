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

/* Returns variable's value
 * by its name */
char	*env_get(char *name)
{
	(void)name;	
	return (NULL);
}

/* If a variable named `name` already exists,
 * its old value is overwritten with `value`.
 * If it does not exist, a new variable named
 * `name` is created with the value `value` */
void	env_set(char *name, char *value)
{
	(void)name;
	(void)value;
}

void	env_unset(char *name)
{
	(void)name;
}

void	env_export(char *name)
{
	(void)name;
}

void	env_print(char *name)
{
	(void)name;
}
