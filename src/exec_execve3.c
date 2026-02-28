#include "exec.h"
#include "shell.h"
#include "builtin.h"
#include "aux_common.h"

#include "error.h"
#include "libft.h"

#include <stdlib.h>

static void	go_to_next_path_comp(char *path, size_t *i);

int	map_exec_errno(int errnum)
{
	if (errnum == ENOENT || errnum == ENOTDIR)
		return (CMD_NOT_LOCATED_ERR);
	if (errnum == EACCES || errnum == EISDIR || errnum == ENOEXEC)
		return (CMD_LOCATED_BUT_NOT_EXEC_ERR);
	return (RET_CMD_FAILURE);
}

char	*resolve_cmd_path(t_shell *msh, char *cmd)
{
	char	*fret;
	char	*str;

	if (!cmd)
		return (NULL);
	str = ft_strdup(cmd);
	if (!str)
	{
		perror("malloc");
		return (NULL);
	}
	if (cmd[0] == '/')
		return (str);
	if (has_slash(cmd))
	{
		fret = get_full_path_from_cwd(cmd);
		if (!fret)
			free(str);
		return (fret);
	}
	fret = resolve_in_path(msh, cmd);
	if (!fret)
		free(str);
	return (fret);
}

char	*resolve_in_path(t_shell *msh, const char *cmd)
{
	size_t	i;
	size_t	start;
	char	*path;
	char	*dir;
	char	*try;

	path = env_get_val(&msh->env, "PATH");
	if (!path || !cmd || !cmd[0])
		return (NULL);
	i = 0;
	while (path[i])
	{
		start = i;
		while (path[i] && path[i] != ':')
			++i;
		dir = ft_substr(path, start, i - start);
		if (!dir)
			return (NULL);
		try = try_path_dir(dir, cmd);
		free(dir);
		if (try)
			return (try);
		go_to_next_path_comp(path, &i);
	}
	return (NULL);
}

static void	go_to_next_path_comp(char *path, size_t *i)
{
	if (path[*i] == ':')
		++(*i);
}

char	*try_path_dir(const char *dir, const char *cmd)
{
	char	*p;

	if (!dir || !cmd)
		return (NULL);
	if (dir[0] == '\0')
		p = join_path(".", cmd);
	else
		p = join_path(dir, cmd);
	if (!p)
		return (NULL);
	if (access(p, X_OK) == 0)
		return (p);
	free(p);
	return (NULL);
}
