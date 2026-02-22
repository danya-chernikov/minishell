#include "exec.h"
#include "shell.h"
#include "builtin.h"
#include "aux_common.h"

#include "error.h"
#include "libft.h"

#include <stdlib.h>

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
		return (get_full_path_from_cwd(cmd));
	return (resolve_in_path(msh, cmd));
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
		if (path[i] == ':')
			++i;
	}
	return (NULL);
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

char	*join_path(const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	size_t	out_len;
	char	*out;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	out_len = dir_len + cmd_len + 2;
	out = (char *)malloc(out_len * sizeof (char));
	if (!out)
		return (NULL);
	ft_strlcpy(out, dir, out_len);
	if (dir_len && out[dir_len - 1] != '/')
		ft_strlcat(out, "/", out_len);
	ft_strlcat(out, cmd, out_len);
	return (out);
}
