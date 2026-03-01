/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:38:11 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:38:11 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "operand.h"

#include "error.h"
#include "libft.h"

static bool	is_n_flag(const char *s);

int	builtin_echo(t_operand *op)
{
	bool	newline;
	int		arg_i;

	newline = true;
	arg_i = 1;
	while (arg_i < op->argc && is_n_flag(op->argv[arg_i]))
	{
		newline = false;
		++arg_i;
	}
	while (arg_i < op->argc)
	{
		write(STDOUT_FILENO, op->argv[arg_i], ft_strlen(op->argv[arg_i]));
		if (arg_i + 1 < op->argc)
			write(STDOUT_FILENO, " ", 1);
		++arg_i;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (RET_CMD_SUCCESS);
}

static bool	is_n_flag(const char *s)
{
	size_t	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (false);
	i = 1;
	while (s[i] == 'n')
		++i;
	if (s[i] == '\0')
		return (true);
	return (false);
}
