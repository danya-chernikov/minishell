/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:04:53 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:04:54 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtin.h"
#include "aux_common.h"

static bool	is_any_builtin_first_part(const char *s);
static bool	is_any_builtin_second_part(const char *s);

bool	is_parent_builtin(const char *s)
{
	if (!s)
		return (false);
	if (strings_equal((char *)s, CD_CMD))
		return (true);
	if (strings_equal((char *)s, EXPORT_CMD))
		return (true);
	if (strings_equal((char *)s, UNSET_CMD))
		return (true);
	if (strings_equal((char *)s, EXIT_CMD))
		return (true);
	return (false);
}

bool	is_any_builtin(const char *s)
{
	bool	res;

	if (!s)
		return (false);
	res = is_any_builtin_first_part(s);
	if (!res)
		res = is_any_builtin_second_part(s);
	return (res);
}

static bool	is_any_builtin_first_part(const char *s)
{
	if (strings_equal((char *)s, ECHO_CMD))
		return (true);
	if (strings_equal((char *)s, CD_CMD))
		return (true);
	if (strings_equal((char *)s, PWD_CMD))
		return (true);
	if (strings_equal((char *)s, EXPORT_CMD))
		return (true);
	if (strings_equal((char *)s, UNSET_CMD))
		return (true);
	if (strings_equal((char *)s, ENV_CMD))
		return (true);
	if (strings_equal((char *)s, PRINTENV_CMD))
		return (true);
	return (false);
}

static bool	is_any_builtin_second_part(const char *s)
{
	if (strings_equal((char *)s, LOCALS_CMD))
		return (true);
	if (strings_equal((char *)s, SET_CMD))
		return (true);
	if (strings_equal((char *)s, EXIT_CMD))
		return (true);
	if (strings_equal((char *)s, TRUE_CMD))
		return (true);
	if (strings_equal((char *)s, FALSE_CMD))
		return (true);
	if (strings_equal((char *)s, NOP_CMD))
		return (true);
	return (false);
}
