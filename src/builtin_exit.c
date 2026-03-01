/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:28:58 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 12:38:35 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "shell.h"
#include "operand.h"

#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

static int	get_exit_code(t_shell *msh, t_operand *op);
static int	get_last_status(t_shell *msh);
static void	exit_now(t_shell *msh, bool f_in_parent, int code);
static int	parse_exit_code(const char *s);

int	builtin_exit(t_shell *msh, t_operand *op, bool f_in_parent)
{
	int	code;

	if (f_in_parent && isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
	if (op->argc >= 2 && !ft_isnum(op->argv[1]))
	{
		print_shell_error(op->argv[1], EXIT_NUM_ARG_REQ);
		exit_now(msh, f_in_parent, CMD_BUILTIN_ERR);
	}
	if (op->argc > 2)
		return (print_shell_error(NULL, EXIT_TOO_MANY_ARGS), CMD_BUILTIN_ERR);
	code = get_exit_code(msh, op);
	exit_now(msh, f_in_parent, code);
	return (code);
}

static int	get_exit_code(t_shell *msh, t_operand *op)
{
	if (op->argc == 2)
		return (parse_exit_code(op->argv[1]));
	return (get_last_status(msh));
}

static int	get_last_status(t_shell *msh)
{
	char	*s;

	s = NULL;
	if (msh && msh->env.vars)
		s = msh->env.vars[PV_RETCODE].value;
	if (!s)
		return (0);
	return ((unsigned char)ft_atoi(s));
}

static void	exit_now(t_shell *msh, bool f_in_parent, int code)
{
	if (f_in_parent)
		msh_free(msh);
	exit(code);
}

static int	parse_exit_code(const char *s)
{
	unsigned int	acc;
	int				sign;
	size_t			i;

	acc = 0;
	sign = 1;
	i = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		++i;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		acc = (acc * 10U + (unsigned int)(s[i] - '0')) % 256U;
		++i;
	}
	if (sign < 0)
		acc = (256U - (acc % 256U)) % 256U;
	return ((int)acc);
}
