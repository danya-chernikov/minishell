#include "exec.h"

#include "error.h"

#include <unistd.h>
#include <linux/limits.h>

#include <stdlib.h>

bool	has_slash(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '/')
			return (true);
		++i;
	}
	return (false);
}

char	*get_full_path_from_cwd(const char *rel_path)
{
	char	cwd[PATH_MAX];
	char	*dir;
	char	*out;

	if (!getcwd(cwd, sizeof (cwd)))
		return (NULL);
	dir = ft_strdup(cwd);
	if (!dir)
		return (NULL);
	out = join_path(dir, rel_path);
	free(dir);
	return (out);
}

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
