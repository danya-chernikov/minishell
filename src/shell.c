/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:17:50 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 01:46:52 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "cmdargs_parser.h"
#include "prompt_parser.h"
#include "expansion.h"

#include "error.h"
#include "libft.h"

#include <stdlib.h>

static int	environment_and_history_init(t_shell *msh, char **env);
static int	prelim_struct_init(t_shell *msh, int argc, char **argv);

int	msh_init(t_shell *msh, int argc, char **argv, char **env)
{
	int		fres;
	bool	f_exit;

	f_exit = false;
	if (prelim_struct_init(msh, argc, argv) != COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	if (environment_and_history_init(msh, env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	fres = cmdargs_parser(msh, &f_exit);
	if (fres != COMMON_SUCCESS)
		return (fres);
	if (f_exit)
		return (MUST_EXIT);
	if (!isatty(STDIN_FILENO))
		if (msh->mode != NONINT_SCRIPT_MODE && msh->mode != NONINT_CMD_MODE)
			msh->mode = NONINT_STDIN_MODE;
	if (msh_set_local_vars(&msh->env, argv) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	fres = msh_set_env_vars(&msh->env);
	if (fres != COMMON_SUCCESS)
		return (fres);
	if (msh_load_configs(msh) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

void	msh_free(t_shell *msh)
{
	exp_free_all_ops_argv(msh->pd);
	parser_free(msh->pd);
	if (msh->pd)
		free(msh->pd);
	if (msh->prompt_inv)
		free(msh->prompt_inv);
	env_free(&msh->env);
	history_free(&msh->history);
	configs_free(&msh->configs);
}

static int	environment_and_history_init(t_shell *msh, char **env)
{
	if (env_init(&msh->env, env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	if (history_init(&msh->history) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	if (msh_init_param_vars(&msh->env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

/* Initialize shell options, configs and parser data  */
static int	prelim_struct_init(t_shell *msh, int argc, char **argv)
{
	msh->mode = INT_MODE;
	msh->env.vars = NULL;
	msh->history.lines = NULL;
	msh->prompt_inv = NULL;
	msh->script = NULL;
	msh->c_cmd = NULL;
	msh->argc = argc;
	msh->argv = argv;
	msh->opts.f_login = false;
	msh->opts.f_verbose = false;
	msh->opts.f_norc = false;
	msh->opts.f_c = false;
	configs_init(&msh->configs);
	msh->pd = (t_parser_data *)ft_calloc(1, sizeof(t_parser_data));
	if (!msh->pd)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
/* Initializes the `t_shell` structure, which represents our
 * minishell and stores all its settings.
 * When we
 * if (!isatty(STDIN_FILENO)) { ... }
 * Here we just determine if print prompt or not
 * if the commands source of our shell is STDIN.
 * Our bash may have 3 command sources
 *     1. bash script.sh			from script
 *     2. bash -c 'commands'		from -c option
 *     3. bash						from STDIN (but we print prompt!)
 *            OR
 *        bash < commands_file		from STDIN (but we do NOT print prompt!)
 *            OR
 *        echo ls | bash | wc -l	from STDIN (but we do NOT print prompt!)
 * So we don't care about stdin stream source of commands in cases 1 and 2
 *     echo ls | bash script.sh
 *     (`ls` will not be executed! only lines from script.sh are executed)
 *
 *     echo ls | bash -c 'commands'
 *     (`ls` will not be executed! only commands after -c will be executed)
 *
 * In all these cases the parent shell who launches
 * bash will handle pipes and redirections! */
