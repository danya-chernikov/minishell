/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdargs_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 04:54:28 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 03:03:27 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdargs_parser.h"
#include "aux_common.h"

#include <stdio.h>
#include <stdlib.h>

static int	parser_loop_first_part(t_shell *msh, int opt_i);
static int	parser_loop_second_part(t_shell *msh, int opt_i);

/* In fact, our command-line argument
 * parser also sets the values of almost
 * all shell parameter variables, except
 * ~ and $?, which will change continuously
 * during the script's execution flow */
int	cmdargs_parser(t_shell *msh)
{
	int	opt_i;
	int	fret;

	get_last_path_comp(msh->argv[0]);
	msh->env.vars[PV_ARGV0].value = ft_strdup(msh->argv[0]);
	opt_i = 1;
	while (opt_i < msh->argc)
	{
		fret = parser_loop_first_part(msh, opt_i);
		if (fret == BREAK)
			break ;
		else if (fret != COMMON_SUCCESS)
			return (fret);
		fret = parser_loop_second_part(msh, opt_i);
		if (fret != COMMON_SUCCESS)
			return (fret);
		++opt_i;
	}
	return (COMMON_SUCCESS);
}

static int	parser_loop_first_part(t_shell *msh, int opt_i)
{
	if (is_help_opt(msh->argv[opt_i]))
	{
		print_help();
		return (BREAK);
	}
	else if (is_version_opt(msh->argv[opt_i]))
	{
		print_version();
		return (BREAK);
	}
	else if (is_verbose_opt(msh->argv[opt_i]))
		msh->opts.f_verbose = true;
	else if (is_login_opt(msh->argv[opt_i]))
	{
		msh->opts.f_login = true;
		if (set_argv0_login(msh) == COMMON_SYS_ERR)
			return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

static int	parser_loop_second_part(t_shell *msh, int opt_i)
{
	if (is_norc_opt(msh->argv[opt_i]))
		msh->opts.f_norc = true;
	else if (is_c_opt(msh->argv[opt_i]))
		return (handle_c_opt(msh, opt_i));
	else
	{
		if (msh->argv[opt_i][0] == '-')
		{
			print_shell_error(msh->argv[opt_i], INV_OPT_ERR_MSG);
			print_help();
			return (SYNTAX_ERR);
		}
		return (handle_script(msh, opt_i));
	}
	return (COMMON_SUCCESS);
}

int	handle_c_opt(t_shell *msh, int opt_i)
{
	msh->opts.f_c = true;
	msh->mode = NONINT_CMD_MODE;
	if (opt_i == msh->argc - 1)
	{
		print_shell_error(C_SHORT_OPT, C_OPT_ERR_MSG);
		return (SYNTAX_ERR);
	}
	if (!erase_quotes(msh->argv[opt_i + 1]))
	{
		print_shell_error(C_SHORT_OPT, SYNTAX_ERR_MSG);
		return (SYNTAX_ERR);
	}
	msh->c_cmd = msh->argv[opt_i + 1];
	return (COMMON_SUCCESS);
}

int	handle_script(t_shell *msh, int opt_i)
{
	int		arg_i;
	size_t	allargv_len;

	msh->mode = NONINT_SCRIPT_MODE;
	msh->script = msh->argv[opt_i];
	if (set_script_args(msh, opt_i, &allargv_len, &arg_i) == SYNTAX_ERR)
		return (SYNTAX_ERR);
	if (msh->env.vars[PV_ARGNUM].value)
		free(msh->env.vars[PV_ARGNUM].value);
	msh->env.vars[PV_ARGNUM].value = ft_itoa(arg_i);
	if (set_allargs_var(msh, allargv_len, arg_i) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}
