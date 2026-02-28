#include "aux_common.h"

#include "env.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

bool	strings_equal(char *str1, char *str2)
{
	if (ft_strlen(str1) == ft_strlen(str2)
		&& !ft_strncmp(str1, str2, ft_strlen(str1)))
	{
		return (true);
	}
	return (false);
}

void	remove_newline(char *str)
{
	if (!str || str[0] == '\0')
		return ;
	if (str[ft_strlen(str) - 1] == '\n')
		str[ft_strlen(str) - 1] = '\0';
}

/* Concatenates the relative path `rel_path`
 * with the current working directory to obtain
 * the full path to the entity represented by
 * `rel_path`. Allocates memory for the returned
 * full path. Returns NULL on failure */
char	*get_full_path(char *rel_path)
{
	char	*full_path;
	char	cwd[PATH_MAX];

	if (!rel_path)
		return (NULL);
	if (rel_path[0] == '/')
	{
		full_path = ft_strdup(rel_path);
		if (!full_path)
		{
			perror("malloc");
			return (NULL);
		}
		return (full_path);
	}
	if (!getcwd(cwd, sizeof (cwd)))
	{
		perror("getcwd");
		return (NULL);
	}
	full_path = join_path(cwd, rel_path);
	return (full_path);
}

/* Does the same as ft_split(), but only for two tokens, i.e. it splits `str`
 * into two substrings, `part1` and `part2`, using the delimiter `delim` */
int	div2_str_by_delim(char *str, char delim, char **part1, char **part2)
{
	size_t	i;

	if (ft_strlen(str) - 1 > MAX_ENV_NAME_LEN + MAX_ENV_VAL_LEN)
		return (COMMON_FAILURE);
	if (div2_str_by_delim_alloc(part1, part2) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	i = 0;
	while (str[i] && str[i] != delim)
	{
		(*part1)[i] = str[i];
		++i;
	}
	(*part1)[i] = '\0';
	++i;
	while (str[i])
	{
		(*part2)[i - ft_strlen(*part1) - 1] = str[i];
		++i;
	}
	(*part2)[i - ft_strlen(*part1) - 1] = '\0';
	return (COMMON_SUCCESS);
}

int	div2_str_by_delim_alloc(char **part1, char **part2)
{
	*part1 = (char *)malloc(MAX_ENV_NAME_LEN * sizeof(char));
	if (!(*part1))
		return (COMMON_SYS_ERR);
	*part2 = (char *)malloc(MAX_ENV_VAL_LEN * sizeof(char));
	if (!(*part2))
	{
		free(*part1);
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
