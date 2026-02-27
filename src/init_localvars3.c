/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_localvars3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 05:09:27 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 15:04:30 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/* MSH_VERSION */
int	set_local_mshversion(t_env *env)
{
	env->vars[SL_MSHVER].name = ft_strdup("MSH_VERSION");
	if (!env->vars[SL_MSHVER].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_MSHVER].value = ft_strdup(MSH_VERSION);
	if (!env->vars[SL_MSHVER].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* HISTFILESIZE */
int	set_local_histfilesize(t_env *env)
{
	env->vars[SL_HFSIZE].name = ft_strdup("HISTFILESIZE");
	if (!env->vars[SL_HFSIZE].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_HFSIZE].value = ft_itoa((int)DEF_HISTFILESIZE);
	if (!env->vars[SL_HFSIZE].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* HISTFILE */
int	set_local_histfile(t_env *env)
{
	env->vars[SL_HFILE].name = ft_strdup("HISTFILE");
	if (!env->vars[SL_HFILE].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_HFILE].value = ft_strdup(DEF_MSH_HIST_PATH);
	if (!env->vars[SL_HFILE].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* HISTSIZE	*/
int	set_local_histsize(t_env *env)
{
	env->vars[SL_HSIZE].name = ft_strdup("HISTSIZE");
	if (!env->vars[SL_HSIZE].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_HSIZE].value = ft_itoa((int)DEF_HISTSIZE);
	if (!env->vars[SL_HSIZE].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* MSH */
int	set_local_msh(t_env *env, char **argv)
{
	env->vars[SL_MSH].name = ft_strdup("MSH");
	if (!env->vars[SL_MSH].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_MSH].value = ft_strdup(argv[0]);
	if (!env->vars[SL_MSH].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
