#include "aux_common.h"

#include "env.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

bool	strings_equal(char *str1, char *str2)
{
	if (ft_strlen(str1) == ft_strlen(str2) &&
		!ft_strncmp(str1, str2, ft_strlen(str1)))
	{
		return (true);
	}
	return (false);
}

void	remove_newline(char *str)
{
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
	size_t	fp_len;

	if (!getcwd(cwd, PATH_MAX))
	{
		perror("getcwd");
		return (NULL);
	}
	fp_len = ft_strlen(cwd) + ft_strlen(rel_path) + 1;
	full_path = (char *)malloc(fp_len * sizeof(char));
	if (!full_path)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(full_path, cwd, fp_len);
	ft_strlcat(full_path, rel_path, fp_len);
	return (full_path);
}

/* If ~ does not exist in `str` returns	a duplicate
 * of `str` on heap. On error returns NULL */
char	*expand_homedir(char *str, char *home)
{
	size_t		new_size;
	char		*new_str;
	char		*tilda;
	ptrdiff_t	pdif;
	
	tilda = ft_strchr(str, '~');
	if (!tilda)
		return (ft_strdup(str));
	// Minus one ~ symbol plus one null-byte annihilate each other
	new_size = ft_strlen(str) + ft_strlen(home); // -1 + 1
	new_str = (char *)malloc(new_size * sizeof(char));
	if (!new_str)
		return (NULL);
	// plus 1 null-terminator	
	pdif = (ptrdiff_t)tilda - (ptrdiff_t)str;
	ft_strlcpy(new_str, str, pdif + 1);
	ft_strlcat(new_str, home, new_size);
	ft_strlcat(new_str, str + pdif + 1, new_size);
	return (new_str);
}

/* Does the same as ft_split(), but only for two tokens, i.e. it splits `str`
 * into two substrings, `part1` and `part2`, using the delimiter `delim` */
int div2_str_by_delim(char *str, char delim, char **part1, char **part2)
{
    size_t  i;

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

bool	is_surrounded_quotes(char *str)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\'') ||
		(str[0] == '"' && str[ft_strlen(str) - 1] == '"'))
	{
		return (true);
	}
	return (false);
}

void	erase_quotes_no_check(char *str)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	if (!str || len < 2)
		return ;
	while (i < len - 2)
	{
		str[i] = str[i + 1];
		++i;
	}
	str[len - 2] = '\0';
}
