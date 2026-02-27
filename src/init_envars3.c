/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envars3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:16:29 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 18:00:40 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#include <stdio.h>
#include <stdlib.h>

static int	set_vars(t_env *env, t_passwd *pwd);
static int	check_malloc_errors(t_env *env);

/* LOGNAME */
int	set_env_logname(t_env *env)
{
	env->vars[SE_LOGNAME].name = ft_strdup("LOGNAME");
	if (!env->vars[SE_LOGNAME].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SE_LOGNAME].value = ft_strdup(UNKNOWN_VALUE);
	if (!env->vars[SE_LOGNAME].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* Sets variables defined by the user
 * in the parent shell session. It may
 * be thought that this causes a memory
 * leak, but in fact env_set() releases
 * `var_name` and/or `var_value`l in case
 * of an internal error */
int	set_rest_env_vars(t_env *env)
{
	int		vi;
	int		res;
	char	*var_name;
	char	*var_value;

	vi = 0;
	while (env->inh_env[vi])
	{
		var_name = NULL;
		var_value = NULL;
		res = div2_str_by_delim(env->inh_env[vi], '=', &var_name, &var_value);
		if (res == COMMON_SYS_ERR)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		else if (res == COMMON_FAILURE)
			return (COMMON_FAILURE);
		res = env_set(env, var_name, var_value, ENV);
		if (res != COMMON_SUCCESS)
			return (res);
		++vi;
	}
	return (COMMON_SUCCESS);
}

/* HOME and USER will always be set
 * if the call succeeds. SHELL may
 * be an empty string if the function
 * fails to determine it */
int	set_env_pwd_user_data(t_env *env)
{
	t_passwd	pwd;
	uid_t		uid;
	int			res;

	uid = (uid_t)ft_atoi(env->vars[SL_UID].value);
	res = ft_getpwuid(&pwd, uid);
	if (res == 1)
	{
		if (set_vars(env, &pwd) == COMMON_SYS_ERR)
			return (free_pwd(&pwd), COMMON_SYS_ERR);
		if (check_malloc_errors(env) == COMMON_SYS_ERR)
			return (free_pwd(&pwd), COMMON_SYS_ERR);
	}
	else
	{
		print_shell_error("ft_getpwuid()", GETPWUID_ERR_MSG);
		return (COMMON_SYS_ERR);
	}
	free_pwd(&pwd);
	return (COMMON_SUCCESS);
}

static int	set_vars(t_env *env, t_passwd *pwd)
{
	env->vars[SE_HOME].name = ft_strdup("HOME");
	if (!env->vars[SE_HOME].name)
		return (COMMON_SYS_ERR);
	env->vars[SE_HOME].value = ft_strdup(pwd->pw_dir);
	if (!env->vars[SE_HOME].value)
		return (COMMON_SYS_ERR);
	env->vars[PV_HOME].value = ft_strdup(env->vars[SE_HOME].value);
	if (!env->vars[PV_HOME].value)
		return (COMMON_SYS_ERR);
	env->vars[SE_USER].name = ft_strdup("USER");
	if (!env->vars[SE_USER].name)
		return (COMMON_SYS_ERR);
	env->vars[SE_USER].value = ft_strdup(pwd->pw_name);
	if (!env->vars[SE_USER].value)
		return (COMMON_SYS_ERR);
	env->vars[SE_SHELL].name = ft_strdup("SHELL");
	if (!env->vars[SE_SHELL].name)
		return (COMMON_SYS_ERR);
	if (ft_strlen(pwd->pw_shell) == 0)
		env->vars[SE_SHELL].value = ft_strdup(UNKNOWN_VALUE);
	else
		env->vars[SE_SHELL].value = ft_strdup(pwd->pw_shell);
	if (!env->vars[SE_SHELL].value)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

/* We do not have to even call free()
 * here cause if case if msh_init()
 * fails in main() we'll call msh_free() */
static	int	check_malloc_errors(t_env *env)
{
	t_senvar	vi;

	if (!env->vars[PV_HOME].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	vi = SE_HOME;
	while (vi <= SE_SHELL)
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
