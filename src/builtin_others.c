/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:22:40 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 13:22:42 by jhvalenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "shell.h"
#include "operand.h"

#include "error.h"

#include <stdio.h>

#include <linux/limits.h>
#include <unistd.h>

int	builtin_pwd(void)
{
	char	buf[PATH_MAX];

	if (!getcwd(buf, sizeof (buf)))
	{
		perror("getcwd");
		return (RET_CMD_FAILURE);
	}
	write(STDOUT_FILENO, buf, ft_strlen(buf));
	write(STDOUT_FILENO, "\n", 1);
	return (RET_CMD_SUCCESS);
}

int	builtin_unset(t_shell *msh, t_operand *op)
{
	int	arg_i;

	arg_i = 1;
	while (arg_i < op->argc)
	{
		env_unset(&msh->env, op->argv[arg_i]);
		++arg_i;
	}
	return (RET_CMD_SUCCESS);
}

int	builtin_env(t_shell *msh)
{
	env_print_env(&msh->env);
	return (RET_CMD_SUCCESS);
}

int	builtin_locals(t_shell *msh)
{
	env_print_locals(&msh->env);
	return (RET_CMD_SUCCESS);
}

int	builtin_set(t_shell *msh)
{
	env_print_all(&msh->env);
	return (RET_CMD_SUCCESS);
}
