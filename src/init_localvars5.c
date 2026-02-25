/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_localvars5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 05:11:10 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 05:11:22 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/* PS1 */
int	set_local_ps1(t_env *env)
{
	env->vars[SL_PS1].name = ft_strdup("PS1");
	env->vars[SL_PS1].value = ft_strdup(DEF_PS1);
	return (COMMON_SUCCESS);
}

/* PS2 */
int	set_local_ps2(t_env *env)
{
	env->vars[SL_PS2].name = ft_strdup("PS2");
	env->vars[SL_PS2].value = ft_strdup(DEF_PS2);
	return (COMMON_SUCCESS);
}

/* PS4 */
int	set_local_ps4(t_env *env)
{
	env->vars[SL_PS4].name = ft_strdup("PS4");
	env->vars[SL_PS4].value = ft_strdup(DEF_PS4);
	return (COMMON_SUCCESS);
}
