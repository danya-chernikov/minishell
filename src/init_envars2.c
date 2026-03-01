/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envars2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:15:37 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:14:09 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#include <stdio.h>
#include <stdlib.h>

/* PATH */
int	set_env_path(t_env *env)
{
	char	*path;

	env->vars[SE_PATH].name = ft_strdup("PATH");
	if (!env->vars[SE_PATH].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	path = getenv("PATH");
	if (!path)
		env->vars[SE_PATH].value = ft_strdup(env_get_def_path());
	else
		env->vars[SE_PATH].value = ft_strdup(path);
	if (!env->vars[SE_PATH].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* SHLVL
 * If it exists in `env->ihn_env`, just copy it and
 * increment it, otherwise create it with the value 1 */
int	set_env_shelevel(t_env *env)
{
	char	*shlvl;
	int		new_shlvl;

	env->vars[SE_SHLVL].name = ft_strdup("SHLVL");
	if (!env->vars[SE_SHLVL].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	shlvl = getenv("SHLVL");
	if (!shlvl)
		env->vars[SE_SHLVL].value = ft_strdup("1");
	else
	{
		new_shlvl = ft_atoi(shlvl);
		env->vars[SE_SHLVL].value = ft_itoa(new_shlvl + 1);
	}
	if (!env->vars[SE_SHLVL].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* PWD */
int	set_env_pwd(t_env *env)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
	{
		perror("getcwd");
		return (COMMON_SYS_ERR);
	}
	env->vars[SE_PWD].name = ft_strdup("PWD");
	if (!env->vars[SE_PWD].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SE_PWD].value = ft_strdup(cwd);
	if (!env->vars[SE_PWD].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

// OLDPWD
int	set_env_oldpwd(t_env *env)
{
	env->vars[SE_OLDPWD].name = ft_strdup("OLDPWD");
	if (!env->vars[SE_OLDPWD].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SE_OLDPWD].value = ft_strdup(env->vars[SE_PWD].value);
	if (!env->vars[SE_OLDPWD].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
