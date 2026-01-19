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
	env->vars[SL_PPID].value = ft_itoa((int)ppid);
	return (COMMON_SUCCESS);
}

// Sets both UID and EUID
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
	// UID
	env->vars[SL_UID].name = ft_strdup("UID");
	env->vars[SL_UID].value = ft_itoa((int)uid);
	return (COMMON_SUCCESS);
}

int	set_local_euid(t_env *env)
{
	// EUID
	env->vars[SL_EUID].name = ft_strdup("EUID");
	env->vars[SL_EUID].value = ft_strdup(env->vars[SL_UID].value);
	return (COMMON_SUCCESS);
}

// It's calculated on access, i.e. when the shell tries to expand it
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
	env->vars[SL_MSHPID].name		= ft_strdup("MSHPID");
	env->vars[SL_MSHPID].value		= ft_itoa((int)pid);
	return (COMMON_SUCCESS);
}

// MSH_SUBSHELL	
int	set_local_mshsubsh(t_env *env)
{
	env->vars[SL_MSHSUBSH].name = ft_strdup("MSH_SUBSHELL");
	env->vars[SL_MSHSUBSH].value = ft_strdup("0");
	return (COMMON_SUCCESS);
}
