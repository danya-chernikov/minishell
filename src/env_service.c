/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_service.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:28:40 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:48:17 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "error.h"

bool	env_exist(t_env *env, char *name)
{
	return (env_get_ptr(env, name));
}

/* Exports the variable named name. If no
 * variable with this name exists, an error
 * is returned. Parameter variables cannot
 * be exported. Only local variables created
 * during the session can be exported.
 * Technically, it is possible to export
 * 'special' local variables defined by our
 * shell, such as BASHPID, BASH_SUBSHELL,
 * BASH_VERSION, ... PPID, UID, and EUID.
 * However, even if these variables are
 * exported, they will be overwritten in
 * child shells immediately. Additionally,
 * descendant shells will read their
 * configuration files, and if a configuration
 * defines a 'special' local variable (except
 * PPID, UID, and EUID, which are always
 * determined internally), its value will be
 * changed */
int	env_export(t_env *env, char *name)
{
	t_env_var	*var;

	var = env_get_ptr(env, name);
	if (!var)
		return (COMMON_FAILURE);
	if (var->type == PARAM)
		return (COMMON_FAILURE);
	var->type = ENV;
	var->f_inherit = true;
	return (COMMON_SUCCESS);
}

size_t	env_count_exported_vars(t_env *env)
{
	size_t	vi;
	size_t	exp_cnt;

	if (!env)
		return (0);
	vi = 0;
	exp_cnt = 0;
	while (vi < env->vars_num)
	{
		if (env->vars[vi].name && env->vars[vi].type == ENV)
			++exp_cnt;
		++vi;
	}
	return (exp_cnt);
}

size_t	env_count_all_vars(t_env *env)
{
	size_t	vi;
	size_t	var_cnt;

	if (!env)
		return (0);
	vi = 0;
	var_cnt = 0;
	while (vi < env->vars_num)
	{
		if (env->vars[vi].name
			&& env->vars[vi].value
			&& env->vars[vi].type != PARAM)
		{
			++var_cnt;
		}
		++vi;
	}
	return (var_cnt);
}

/* I'm tired of creating new files. It looks
 * messy.. so I just leave it here for now.
 * This function is used by set_existing_var() */
void	check_var_type(t_env_var *var, t_var_type type)
{
	if (type == ENV || var->type == ENV)
	{
		var->type = ENV;
		var->f_inherit = true;
	}
	else if (var->type != PARAM)
	{
		var->type = LOCAL;
		var->f_inherit = false;
	}
}
