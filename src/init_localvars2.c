/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_localvars2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 05:08:07 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 15:04:07 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	set_local_ppid(t_env *env)
{
	pid_t	ppid;
	int		res;

	res = ft_getppid(&ppid);
	if (res == -1)
	{
		print_shell_error("getppid()", GETPPID_ERR_MSG);
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_PPID].name = ft_strdup("PPID");
	if (!env->vars[SL_PPID].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_PPID].value = ft_itoa((int)ppid);
	if (!env->vars[SL_PPID].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* Sets both UID and EUID */
int	set_local_uid(t_env *env)
{
	uid_t	uid;
	int		res;

	res = ft_getuid(&uid);
	if (res == -1)
	{
		print_shell_error("getuid()", GETUID_ERR_MSG);
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_UID].name = ft_strdup("UID");
	if (!env->vars[SL_UID].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_UID].value = ft_itoa((int)uid);
	if (!env->vars[SL_UID].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

int	set_local_euid(t_env *env)
{
	env->vars[SL_EUID].name = ft_strdup("EUID");
	if (!env->vars[SL_EUID].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_EUID].value = ft_strdup(env->vars[SL_UID].value);
	if (!env->vars[SL_EUID].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* It's calculated on access, i.e.
 * when the shell tries to expand it */
int	set_local_mshpid(t_env *env)
{
	pid_t	pid;
	int		res;

	res = ft_getpid(&pid);
	if (res == -1)
	{
		print_shell_error("getpid()", GETPID_ERR_MSG);
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_MSHPID].name = ft_strdup("MSHPID");
	if (!env->vars[SL_MSHPID].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_MSHPID].value = ft_itoa((int)pid);
	if (!env->vars[SL_MSHPID].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* MSH_SUBSHELL	*/
int	set_local_mshsubsh(t_env *env)
{
	env->vars[SL_MSHSUBSH].name = ft_strdup("MSH_SUBSHELL");
	if (!env->vars[SL_MSHSUBSH].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_MSHSUBSH].value = ft_strdup("0");
	if (!env->vars[SL_MSHSUBSH].value)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
