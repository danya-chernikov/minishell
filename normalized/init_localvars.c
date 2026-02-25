/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_localvars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 05:06:58 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 05:07:34 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#include <stdio.h>

static void	prelim_vars_init(t_env *env);
static void	set_vars(t_env *env, char **argv, int *ret_codes);
static int	check_mem_errors(t_env *env);

int	msh_set_local_vars(t_env *env, char **argv)
{
	int	ret_codes[SLOCAL_VARS_NUM];
	int	ret_i;

	prelim_vars_init(env);
	set_vars(env, argv, ret_codes);
	ret_i = 0;
	while (ret_i < SLOCAL_VARS_NUM)
	{
		if (ret_codes[ret_i] == COMMON_SYS_ERR)
			return (COMMON_SYS_ERR);
		++ret_i;
	}
	env->vars_num += SLOCAL_VARS_NUM;
	if (check_mem_errors(env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

static void	prelim_vars_init(t_env *env)
{
	t_slocalvar	vi;

	vi = SL_PPID;
	while (vi < SL_MSHSUBSH)
	{
		env->vars[vi].type = LOCAL;
		env->vars[vi].f_readonly = true;
		env->vars[vi].f_inherit = false;
		env->vars[vi].name = NULL;
		env->vars[vi].value = NULL;
		++vi;
	}
	while (vi < PARAM_VARS_NUM + SLOCAL_VARS_NUM)
	{
		env->vars[vi].type = LOCAL;
		env->vars[vi].f_readonly = false;
		env->vars[vi].f_inherit = false;
		env->vars[vi].name = NULL;
		env->vars[vi].value = NULL;
		++vi;
	}
}

static void	set_vars(t_env *env, char **argv, int *ret_codes)
{
	ret_codes[0] = set_local_ppid(env);
	ret_codes[1] = set_local_uid(env);
	ret_codes[2] = set_local_euid(env);
	ret_codes[3] = set_local_mshpid(env);
	ret_codes[4] = set_local_mshsubsh(env);
	ret_codes[5] = set_local_mshversion(env);
	ret_codes[6] = set_local_histfilesize(env);
	ret_codes[7] = set_local_histfile(env);
	ret_codes[8] = set_local_histsize(env);
	ret_codes[9] = set_local_msh(env, argv);
	ret_codes[10] = set_local_hostname(env);
	ret_codes[11] = set_local_hosttype(env);
	ret_codes[12] = set_local_ostype(env);
	ret_codes[13] = set_local_hostmach(env);
	ret_codes[14] = set_local_ps1(env);
	ret_codes[15] = set_local_ps2(env);
	ret_codes[16] = set_local_ps4(env);
}

static int	check_mem_errors(t_env *env)
{
	t_slocalvar	vi;

	vi = SL_PPID;
	while (vi < SL_PPID + SLOCAL_VARS_NUM)
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
