/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:59:18 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 23:44:11 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/* Extracts a single directory from the PATH string and checks if the
 * command exists within it. Updates the index for the next iteration.*/
char	*resolve_in_path(t_shell *msh, const char *cmd)
{
	size_t	i;
	char	*path;
	char	*res;

	path = env_get_val(&msh->env, "PATH");
	if (!path || !cmd || !cmd[0])
		return (NULL);
	i = 0;
	while (path[i])
	{
		res = check_next_path(path, &i, cmd);
		if (res)
			return (res);
	}
	return (NULL);
}

/* Extracts a single directory from the PATH string and checks if the
 * command exists within it. Updates the index for the next iteration.*/
char	*check_next_path(const char *path, size_t *i, const char *cmd)
{
	size_t	start;
	char	*dir;
	char	*try;

	start = *i;
	while (path[*i] && path[*i] != ':')
		(*i)++;
	dir = ft_substr(path, start, *i - start);
	if (!dir)
		return (NULL);
	try = try_path_dir(dir, cmd);
	free(dir);
	if (path[*i] == ':')
		(*i)++;
	return (try);
}

/* Attempts to find a command by appending it to a specific directory path.
 * Returns the allocated full path if executable, otherwise NULL.*/
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
