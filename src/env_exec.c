/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:25:00 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 11:25:06 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "shell.h"

#include "libft.h"
#include "error.h"

#include <stdlib.h>

static int	copy_one_var(t_env *dst, t_env_var *var, t_var_type vartype);

/* Take all variables from `src` (except PARAM and empty)
 * and copies them into `dst` as LOCAL variables */
int	env_apply_as_local(t_env *dst, t_env *src)
{
	size_t	vi;
	int		fret;

	if (!dst || !src)
		return (COMMON_SUCCESS);
	vi = 0;
	while (vi < src->vars_num)
	{
		fret = copy_one_var(dst, &src->vars[vi], LOCAL);
		if (fret != COMMON_SUCCESS)
			return (fret);
		++vi;
	}
	return (COMMON_SUCCESS);
}

/* Does the same as `env_apply_as_local()`
 * but copies variables as ENV */
int	env_apply_as_env(t_env *dst, t_env *src)
{
	size_t	vi;
	int		fret;

	if (!dst || !src)
		return (COMMON_SUCCESS);
	vi = 0;
	while (vi < src->vars_num)
	{
		fret = copy_one_var(dst, &src->vars[vi], ENV);
		if (fret != COMMON_SUCCESS)
			return (fret);
		++vi;
	}
	return (COMMON_SUCCESS);
}

/* Copy one variable `var` into `dst` as a variable of the `vartype` type */
static int	copy_one_var(t_env *dst, t_env_var *var, t_var_type vartype)
{
	char	*name;
	char	*val;

	if (!var->name || !var->value)
		return (COMMON_SUCCESS);
	if (var->type == PARAM)
		return (COMMON_SUCCESS);
	name = ft_strdup(var->name);
	if (!name)
		return (COMMON_SYS_ERR);
	val = ft_strdup(var->value);
	if (!val)
	{
		free(name);
		return (COMMON_SYS_ERR);
	}
	if (env_set(dst, name, val, vartype) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}
