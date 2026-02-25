/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:23:13 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 11:24:42 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
 * variables array.
 *     inh_env - parent's inherited environment.
 *				 May be passed NULL value if we
 *				 do not have it */
int	env_init(t_env *env, char **inh_env)
{
	size_t	i;

	if (!env)
		return (COMMON_SUCCESS);
	env->vars_num = 0;
	env->inh_env = inh_env;
	env->vars = malloc(MAX_TOTAL_VARS_NUM * sizeof (t_env_var));
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

	if (!env)
		return ;
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
			env->vars[i++].value = NULL;
		}
	}
	if (env->vars)
	{
		free(env->vars);
		env->vars = NULL;
	}
	env->vars_num = 0;
	env->inh_env = NULL;
}
