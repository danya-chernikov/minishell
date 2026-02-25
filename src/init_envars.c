/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:13:53 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 11:14:00 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#include <stdio.h>

static void	prelim_vars_init(t_env *env);
static int	check_mem_errors(t_env *env);

/* In addition to setting special environment
 * variables, this function also copies all other
 * non-special variables from the inherited
 * environment. At this stage, we check whether
 * any environment variable has a name that matches
 * one of the special local variable names. If such
 * a variable exists, it means only one thing: the
 * user deleted one or more special local variables,
 * then recreated variables with the same names,
 * assigned them values, exported them, and finally
 * launched another minishell instance (which is us).
 * In this case, we must avoid creating environment
 * variables whose names match the names of special
 * local variables (cause we already have created
 * our own special local variables in current session) */
int	msh_set_env_vars(t_env *env)
{
	int	ret;

	prelim_vars_init(env);
	set_env_path(env);
	set_env_shelevel(env);
	if (set_env_pwd(env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	set_env_oldpwd(env);
	if (set_env_pwd_user_data(env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	set_env_logname(env);
	env->vars_num += SENV_VARS_NUM;
	ret = set_rest_env_vars(env);
	if (ret != COMMON_SUCCESS)
		return (ret);
	if (check_mem_errors(env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

static void	prelim_vars_init(t_env *env)
{
	t_senvar	vi;

	vi = SE_PATH;
	while (vi < SE_PATH + SENV_VARS_NUM)
	{
		env->vars[vi].type = ENV;
		env->vars[vi].f_readonly = false;
		env->vars[vi].f_inherit = true;
		env->vars[vi].name = NULL;
		env->vars[vi].value = NULL;
		++vi;
	}
}

/* Check for malloc() errors */
static int	check_mem_errors(t_env *env)
{
	t_senvar	vi;

	vi = SE_PATH;
	while (vi < SE_PATH + SENV_VARS_NUM)
	{
		if (!env->vars[vi].name || !env->vars[vi].value)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++vi;
	}
	return (COMMON_SUCCESS);
}
