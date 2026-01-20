#include "shell.h"

// MSH_VERSION	
int	set_local_mshversion(t_env *env)
{
	env->vars[SL_MSHVER].name = ft_strdup("MSH_VERSION");
	env->vars[SL_MSHVER].value = ft_strdup(MSH_VERSION);
	return (COMMON_SUCCESS);
}

// HISTFILESIZE
int	set_local_histfilesize(t_env *env)
{
	env->vars[SL_HFSIZE].name = ft_strdup("HISTFILESIZE");
	env->vars[SL_HFSIZE].value = ft_itoa((int)DEF_HISTFILESIZE);
	return (COMMON_SUCCESS);
}

// HISTFILE
int	set_local_histfile(t_env *env)
{
	env->vars[SL_HFILE].name = ft_strdup("HISTFILE");
	env->vars[SL_HFILE].value = ft_strdup(DEF_MSH_HIST_PATH);
	return (COMMON_SUCCESS);
}

// HISTSIZE	
int	set_local_histsize(t_env *env)
{
	env->vars[SL_HSIZE].name = ft_strdup("HISTSIZE");
	env->vars[SL_HSIZE].value = ft_itoa((int)DEF_HISTSIZE);
	return (COMMON_SUCCESS);
}

// MSH
int	set_local_msh(t_env *env, char **argv)
{
	env->vars[SL_MSH].name = ft_strdup("MSH");
	env->vars[SL_MSH].value = ft_strdup(argv[0]);
	return (COMMON_SUCCESS);
}
