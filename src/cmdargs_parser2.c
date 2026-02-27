/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdargs_parser2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 04:54:29 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 01:39:29 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdargs_parser.h"
#include "shell.h"

#include "error.h"
#include "libft.h"

#include <stdio.h>
#include <stdlib.h>

int	set_script_args(t_shell *msh, int opt_i, size_t *allargv_len, int *arg_i)
{
	if (!msh->env.vars[PV_ARGV0].value)
	{
		msh->env.vars[PV_ARGV0].value = ft_strdup(msh->argv[opt_i]);
		if (!msh->env.vars[PV_ARGV0].value)
			return (perror("malloc"), COMMON_SYS_ERR);
	}
	*allargv_len = 0;
	*arg_i = 1;
	while (opt_i + *arg_i < msh->argc && *arg_i - 1 < SCRIPT_ARGS_NUM)
	{
		if (!erase_quotes(msh->argv[opt_i + *arg_i]))
		{
			print_shell_error(C_SHORT_OPT, SYNTAX_ERR_MSG);
			return (SYNTAX_ERR);
		}
		msh->env.vars[PV_ARGV0 + *arg_i].value
			= ft_strdup(msh->argv[opt_i + *arg_i]);
		*allargv_len += ft_strlen(msh->argv[opt_i + *arg_i]);
		++(*arg_i);
	}
	--(*arg_i);
	return (COMMON_SUCCESS);
}

int	set_allargs_var(t_shell *msh, size_t allargv_len, int arg_i)
{
	char	*allargv;
	int		i;

	if (arg_i > 0)
	{
		allargv_len += arg_i - 1;
		++allargv_len;
		allargv = malloc(allargv_len * sizeof(char));
		if (!allargv)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		ft_strlcpy(allargv, msh->env.vars[PV_ARGV1].value, allargv_len);
		i = 1;
		while (i < arg_i)
		{
			ft_strlcat(allargv, " ", allargv_len);
			ft_strlcat(allargv, msh->env.vars[PV_ARGV1 + i].value, allargv_len);
			++i;
		}
		msh->env.vars[PV_ALLARGS].value = allargv;
	}
	return (COMMON_SUCCESS);
}

int	set_argv0_login(t_shell *msh)
{
	char	*new_argv0;
	size_t	new_size;

	get_last_path_comp(msh->argv[0]);
	new_size = ft_strlen(msh->argv[0]) + 2;
	new_argv0 = malloc(new_size * sizeof(char));
	if (!new_argv0)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	new_argv0[0] = '-';
	ft_strlcpy(new_argv0 + sizeof(char), msh->argv[0], new_size - 1);
	if (msh->env.vars[PV_ARGV0].value)
		free(msh->env.vars[PV_ARGV0].value);
	msh->env.vars[PV_ARGV0].value = new_argv0;
	return (COMMON_SUCCESS);
}

/* Removes possible quotes
 * surrounding `str`. If
 * it's impossible to remove
 * the quotes correctly, it
 * reports an error.
 * For example:
 *     bash -c 'ls -> error
 *     bash -c ls" -> error
 *     bash -c 'ls" -> error
 *     bash -c 'ls' -> ok
 *     bash -c "ls" -> ok */
int	erase_quotes(char *str)
{
	size_t	i;

	if ((str[0] == '\'' && str[ft_strlen(str) - 1] != '\'')
		|| (str[0] == '"' && str[ft_strlen(str) - 1] != '"'))
	{
		return (COMMON_FAILURE);
	}
	if ((str[ft_strlen(str) - 1] == '\'' && str[0] != '\'')
		|| (str[ft_strlen(str) - 1] == '"' && str[0] != '"'))
	{
		return (COMMON_FAILURE);
	}
	if (str[0] == '\'' || str[0] == '"')
	{
		str[ft_strlen(str) - 1] = '\0';
		i = 0;
		while (i < ft_strlen(str) - 1)
		{
			str[i] = str[i + 1];
			++i;
		}
	}
	return (COMMON_SUCCESS);
}

/* Eliminates all except the last path component.
 * For example:
 *     path = "/home/dchernik/Downloads/circle03/minishell/src/minishell"
 *     get_last_path_comp(path);
 *     path = "minishell" */
void	get_last_path_comp(char *path) // Give it better name
{
	int	i;
	int	j;
	int	slash_ind;
	int	path_len;

	i = 0;
	slash_ind = -1;
	path_len = (int)ft_strlen(path);
	while (i < path_len)
	{
		if (path[i] == '/')
			slash_ind = i;
		++i;
	}
	if (slash_ind == -1)
		return ;
	j = 0;
	i = slash_ind + 1;
	while (i < path_len)
	{
		path[j] = path[i];
		++i;
		++j;
	}
	path[j] = '\0';
}
