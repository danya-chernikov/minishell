/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdargs_parser3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 04:54:30 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 01:09:38 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdargs_parser.h"
#include "shell.h"

#include "error.h"
#include "libft.h"

bool	is_help_opt(char *opt)
{
	if (!ft_strncmp(opt, HELP_LONG_OPT, ft_strlen(HELP_LONG_OPT)))
		return (true);
	return (false);
}

bool	is_version_opt(char *opt)
{
	if (!ft_strncmp(opt, VERSION_LONG_OPT, ft_strlen(VERSION_LONG_OPT)))
		return (true);
	return (false);
}

bool	is_verbose_opt(char *opt)
{
	if (!ft_strncmp(opt, VERBOSE_LONG_OPT, ft_strlen(VERBOSE_LONG_OPT))
		|| !ft_strncmp(opt, VERBOSE_SHORT_OPT, ft_strlen(VERBOSE_SHORT_OPT)))
		return (true);
	return (false);
}

bool	is_login_opt(char *opt)
{
	if (!ft_strncmp(opt, LOGIN_LONG_OPT, ft_strlen(LOGIN_LONG_OPT))
		|| !ft_strncmp(opt, LOGIN_SHORT_OPT, ft_strlen(LOGIN_SHORT_OPT)))
		return (true);
	return (false);
}

bool	is_norc_opt(char *opt)
{
	if (!ft_strncmp(opt, NORC_LONG_OPT, ft_strlen(NORC_LONG_OPT)))
		return (true);
	return (false);
}
