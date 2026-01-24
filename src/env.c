#include "env.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* Maybe it would be better to use a singly
 * linked list here as variable storage,
 * but I'm not quite sure.. it's heavy anyway.
 * In practice, we are never going to have
 * duplicates in our variables array, because
 * when we unset a variable we simply free its
 * value and set it to NULL, we do not touch
 * its name. So if the user later creates a
 * new variable with the same name, the value
 * will be assigned to the same index in the
 * variables array. */
int	env_init(t_env *env)
{
	size_t	i;

	env->vars = malloc(MAX_TOTAL_VARS_NUM * sizeof *env->vars);
	if (!env->vars)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	i = 0;
	while (i < MAX_TOTAL_VARS_NUM)
	{
		env->vars[i].name = NULL;
		env->vars[i].value = NULL;
		++i;
	}
	return (COMMON_SUCCESS);
}

void	env_free(t_env *env)
{
	size_t	i;

	i = 0;
	while (i < MAX_TOTAL_VARS_NUM)
	{
		if (env->vars[i].name)
		{
			free(env->vars[i].name);
			env->vars[i].name = NULL;
		}
		if (env->vars[i].value)
		{
			free(env->vars[i].value);
			env->vars[i].value = NULL;
		}
		++i;
	}
	if (env->vars)
		free(env->vars);
}
