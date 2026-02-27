/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_localvars5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:59:24 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 17:59:25 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/* PS1 */
int	set_local_ps1(t_env *env)
{
	env->vars[SL_PS1].name = ft_strdup("PS1");
	if (!env->vars[SL_PS1].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_PS1].value = ft_strdup(DEF_PS1);
	if (!env->vars[SL_PS1].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* PS2 */
int	set_local_ps2(t_env *env)
{
	env->vars[SL_PS2].name = ft_strdup("PS2");
	if (!env->vars[SL_PS2].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_PS2].value = ft_strdup(DEF_PS2);
	if (!env->vars[SL_PS2].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* PS4 */
int	set_local_ps4(t_env *env)
{
	env->vars[SL_PS4].name = ft_strdup("PS4");
	if (!env->vars[SL_PS4].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_PS4].value = ft_strdup(DEF_PS4);
	if (!env->vars[SL_PS4].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
