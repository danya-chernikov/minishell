/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envars2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:15:37 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 11:16:21 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#include <stdio.h>
#include <stdlib.h>

/* PATH */
void	set_env_path(t_env *env)
{
	char	*path;

	env->vars[SE_PATH].name = ft_strdup("PATH");
	path = getenv("PATH");
	if (!path)
		env->vars[SE_PATH].value = ft_strdup(DEF_PATH);
	else
		env->vars[SE_PATH].value = ft_strdup(path);
}

/* SHLVL
 * If it exists in `env->ihn_env`, just copy it and
 * increment it, otherwise create it with the value 1 */
void	set_env_shelevel(t_env *env)
{
	char	*shlvl;
	int		new_shlvl;

	env->vars[SE_SHLVL].name = ft_strdup("SHLVL");
	shlvl = getenv("SHLVL");
	if (!shlvl)
		env->vars[SE_SHLVL].value = ft_strdup("1");
	else
	{
		new_shlvl = ft_atoi(shlvl);
		env->vars[SE_SHLVL].value = ft_itoa(new_shlvl + 1);
	}
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
	env->vars[SE_PWD].value = ft_strdup(cwd);
	return (COMMON_SUCCESS);
}

// OLDPWD
void	set_env_oldpwd(t_env *env)
{
	env->vars[SE_OLDPWD].name = ft_strdup("OLDPWD");
	env->vars[SE_OLDPWD].value = ft_strdup(env->vars[SE_PWD].value);
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
	if (res)
	{
		env->vars[SE_HOME].name = ft_strdup("HOME");
		env->vars[SE_HOME].value = ft_strdup(pwd.pw_dir);
		env->vars[PV_HOME].value = ft_strdup(env->vars[SE_HOME].value);
		env->vars[SE_USER].name = ft_strdup("USER");
		env->vars[SE_USER].value = ft_strdup(pwd.pw_name);
		env->vars[SE_SHELL].name = ft_strdup("SHELL");
		if (ft_strlen(pwd.pw_shell) == 0)
			env->vars[SE_SHELL].value = ft_strdup(UNKNOWN_VALUE);
		else
			env->vars[SE_SHELL].value = ft_strdup(pwd.pw_shell);
	}
	else
	{
		print_shell_error("ft_getpwuid()", GETPWUID_ERR_MSG);
		return (COMMON_SYS_ERR);
	}
	return (free_pwd(&pwd), COMMON_SUCCESS);
}
