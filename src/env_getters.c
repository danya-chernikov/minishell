/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_getters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:25:21 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 21:57:47 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/* Returns variable's value by its name. If a variable
 * with such name does not exist returns NULL */
char	*env_get_val(t_env *env, char *name)
{
	size_t	i;	

	if (!env || !name)
		return (NULL);
	i = 0;
	while (i < env->vars_num)
	{
		if (env->vars[i].name && strings_equal(name, env->vars[i].name))
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
	size_t		i;

	if (!env || !name)
		return (NULL);
	i = 0;
	while (i < env->vars_num)
	{
		if (env->vars[i].name && strings_equal(name, env->vars[i].name))
			return (&env->vars[i]);
		++i;
	}
	return (NULL);
}
