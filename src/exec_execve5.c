#include "exec.h"
#include "env.h"

#include "error.h"

#include <unistd.h>
#include <linux/limits.h>

#include <stdlib.h>

/* Check whether the variable `name` exists among
 * the strings `envp` */
bool	envp_has_name(char **envp, const char *name)
{
	size_t	i;

	if (!envp || !name)
		return (false);
	i = 0;
	while (envp[i])
	{
		if (envp_name_eq(envp[i], name))
			return (true);
		++i;
	}
	return (false);
}

/* Compares variable's name in the string
 * `envs` with the given string `name` */
bool	envp_name_eq(const char *envs, const char *name)
{
	size_t	i;

	i = 0;
	while (envs[i] && envs[i] != '=')
	{
		if (!name[i] || name[i] != envs[i])
			return (false);
		++i;
	}
	if (envs[i] == '=' && name[i] == '\0')
		return (true);
	return (false);
}

void	free_envp(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		++i;
	}
	free(envp);
}

void	child_set_default_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
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
		if (env->vars[i].name && env->vars[i].value &&
			env->vars[i].type != PARAM)
			++cnt;
		++i;
	}
	return (cnt);
}
