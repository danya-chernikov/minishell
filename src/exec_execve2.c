/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:14:46 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:14:46 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "shell.h"
#include "token.h"
#include "operand.h"
#include "builtin.h"
#include "expansion.h"
#include "aux_common.h"

#include "error.h"

#include <stdlib.h>

int	prepare_operand(t_shell *msh, t_token *token)
{
	t_operand	*op;
	int			fres;

	if (!token || token->type != OPERAND || !token->op)
		return (COMMON_FAILURE);
	op = token->op;
	fres = prep_reset_operand(op);
	if (fres != COMMON_SUCCESS)
		return (fres);
	fres = exp_alloc_argv(op);
	if (fres != COMMON_SUCCESS)
		return (fres);
	fres = do_all_expansions_assignments(msh, token);
	if (fres != COMMON_SUCCESS)
		return (prep_cleanup_failed_expand(op, fres));
	return (COMMON_SUCCESS);
}

char	**build_envp_for_operand(t_shell *msh, t_operand *op)
{
	int		fret;
	size_t	ei;
	size_t	total;
	char	**envp;

	total = 0;
	total += count_operand_envp(op->my_env);
	total += count_shell_envp(msh, op->my_env);
	envp = malloc((total + 1) * sizeof (*envp));
	if (!envp)
		return (NULL);
	ei = 0;
	envp[0] = NULL;
	fret = copy_operand_env(op, envp, &ei);
	if (fret != COMMON_SUCCESS)
		return (free_envp_partial(envp, ei), NULL);
	fret = copy_shell_env(msh, envp, &ei);
	if (fret != COMMON_SUCCESS)
		return (free_envp_partial(envp, ei), NULL);
	return (envp);
}

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
