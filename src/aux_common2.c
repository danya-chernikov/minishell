/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_common2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:27:33 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:27:33 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aux_common.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

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
	new_size = ft_strlen(str) + ft_strlen(home);
	new_str = (char *)malloc(new_size * sizeof(char));
	if (!new_str)
		return (NULL);
	pdif = (ptrdiff_t)tilda - (ptrdiff_t)str;
	ft_strlcpy(new_str, str, pdif + 1);
	ft_strlcat(new_str, home, new_size);
	ft_strlcat(new_str, str + pdif + 1, new_size);
	return (new_str);
}

bool	contains_quote(char *str)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(str))
	{
		if (str[i] == '\'' || str[i] == '"')
			return (true);
		++i;
	}
	return (false);
}

bool	is_variable_name_correct(char *var_name)
{
	size_t	i;

	if (!var_name || var_name[0] == '\0')
		return (false);
	if (!(ft_isalpha(var_name[0]) || var_name[0] == '_'))
		return (false);
	i = 1;
	while (var_name[i] != '\0')
	{
		if (!(ft_isalnum(var_name[i]) || var_name[i] == '_'))
			return (false);
		++i;
	}
	return (true);
}

/* REVELATION: only variables we create cannot start
 * with a digit, variables we expand actually can
 * (for example $0, or $1, $2, ... , $9).
 * So we call this function when we expand variables */
bool	is_varname_symbol_permitted(char symbol)
{
	if (ft_isalnum(symbol) || symbol == '_')
		return (true);
	return (false);
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
