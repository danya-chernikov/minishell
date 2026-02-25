/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:16:58 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 13:20:50 by jhvalenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "shell.h"
#include "operand.h"

#include <stdlib.h>

static int	parse_exit_code(const char *s);

int	builtin_exit(t_shell *msh, t_operand *op, bool f_in_parent)
{
	int		code;

	code = 0;
	if (f_in_parent && isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
	if (op->argc >= 2 && !ft_isnum(op->argv[1]))
	{
		print_shell_error(op->argv[1], EXIT_NUM_ARG_REQ);
		if (f_in_parent)
			msh_free(msh);
		exit(CMD_BUILTIN_ERR);
	}
	if (op->argc > 2)
	{
		print_shell_error(NULL, EXIT_TOO_MANY_ARGS);
		return (CMD_BUILTIN_ERR);
	}
	if (op->argc == 2)
		code = parse_exit_code(op->argv[1]);
	else
		code = get_last_exit_status(msh);
	if (f_in_parent)
		msh_free(msh);
	exit(code);
}

/* Retrieves the last exit status from the shell's environment variables.
 * Returns 0 if the environment or the specific variable is not found.*/
static int	get_last_exit_status(t_shell *msh)
{
	char	*s;

	s = NULL;
	if (msh && msh->env.vars)
		s = msh->env.vars[PV_RETCODE].value;
	if (s)
		return ((unsigned char)ft_atoi(s));
	return (0);
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
