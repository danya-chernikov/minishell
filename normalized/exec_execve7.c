/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve7.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:38:00 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 00:38:01 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

#include "error.h"

#include <stdlib.h>

void	free_envp_partial(char **envp, size_t n)
{
	while (n > 0)
	{
		--n;
		free(envp[n]);
	}
	free(envp);
}

int	copy_operand_env(t_operand *op, char **envp, size_t *ei)
{
	size_t	mi;

	mi = 0;
	while (op->my_env && mi < op->my_env->vars_num)
	{
		if (op->my_env->vars[mi].name && op->my_env->vars[mi].value
			&& op->my_env->vars[mi].type != PARAM)
		{
			envp[*ei] = key_value_to_str(op->my_env->vars[mi].name,
					op->my_env->vars[mi].value);
			if (!envp[*ei])
				return (COMMON_SYS_ERR);
			++(*ei);
			envp[*ei] = NULL;
		}
		++mi;
	}
	return (COMMON_SUCCESS);
}

int	copy_shell_env(t_shell *msh, char **envp, size_t *ei)
{
	size_t	mi;

	mi = 0;
	while (mi < msh->env.vars_num)
	{
		if (msh->env.vars[mi].name && msh->env.vars[mi].type == ENV
			&& !envp_has_name(envp, msh->env.vars[mi].name))
		{
			envp[*ei] = key_value_to_str(msh->env.vars[mi].name,
					msh->env.vars[mi].value);
			if (!envp[*ei])
				return (COMMON_SYS_ERR);
			++(*ei);
			envp[*ei] = NULL;
		}
		++mi;
	}
	return (COMMON_SUCCESS);
}

size_t	count_operand_envp(t_env *env)
{
	size_t	i;
	size_t	cnt;

	if (!env)
		return (0);
	i = 0;
	cnt = 0;
	while (i < env->vars_num)
	{
		if (env->vars[i].name && env->vars[i].value
			&& env->vars[i].type != PARAM)
			++cnt;
		++i;
	}
	return (cnt);
}

size_t	count_shell_envp(t_shell *msh, t_env *op_env)
{
	size_t	i;
	size_t	cnt;

	i = 0;
	cnt = 0;
	while (i < msh->env.vars_num)
	{
		if (msh->env.vars[i].name
			&& msh->env.vars[i].type == ENV
			&& (!op_env || !env_exist(op_env, msh->env.vars[i].name)))
			++cnt;
		++i;
	}
	return (cnt);
}
