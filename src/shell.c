#include "shell.h"
#include "cmdargs_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void	prelim_struct_init(t_shell *msh, int argc, char **argv, char **env);

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
int	msh_init(t_shell *msh, int argc, char **argv, char **env)
{
	int	res;

	prelim_struct_init(msh, argc, argv, env);

	// Allocate environmental variables
	if (env_init(&msh->env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);

	// Init history	
	if (history_init(&msh->history) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);

	// Initialize shell parameters
	if (msh_init_param_vars(&msh->env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);

	// Parsing the command-line arguments
	// of our shell. At this stage, we can
	// determine the shell's mode and set
	// some parameter variables
	res = cmdargs_parser(msh);
	if (res == COMMON_FAILURE)
		return (COMMON_FAILURE);
	else if (res != COMMON_SUCCESS) // The code we should transfer to the caller
		return (res);

	if (!isatty(STDIN_FILENO)) // If shell is not connected to any terminal
	{
		if (msh->mode != NONINT_SCRIPT_MODE && msh->mode != NONINT_CMD_MODE)
			msh->mode = NONINT_STDIN_MODE; // We do not have to print prompt!
	}

	/* Set local and environment variables */
	if (msh_set_local_vars(&msh->env, argv) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	if (msh_set_env_vars(&msh->env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);

	return (COMMON_SUCCESS);
}

static void	prelim_struct_init(t_shell *msh, int argc, char **argv, char **env)
{
	// By default, let's think our
	// shell will be interactive
	msh->mode = INT_MODE;
	msh->env.vars = NULL;
	msh->history.lines = NULL;
	msh->script = NULL;
	msh->c_cmd = NULL;
	// Assign arguments of main()
	msh->argc = argc;
	msh->argv = argv;
	msh->env.inh_env = env;
	// Init shell options
	msh->opts.f_login = false;
	msh->opts.f_verbose = false;
	msh->opts.f_norc = false;
	msh->opts.f_c = false;
}

void	msh_free_all_vars(t_env *env)
{
	size_t	i;

	i = 0;
	while (i < MAX_TOTAL_VARS_NUM)
	{
		if (env->vars[i].name)
		{
			free(env->vars[i].name);
			env->vars[i].name = NULL;
		}
		if (env->vars[i].value)
		{
			free(env->vars[i].value);
			env->vars[i].value = NULL;
		}
		++i;
	}
}

void	msh_free(t_shell *msh)
{
	msh_free_all_vars(&msh->env);
	if (msh->env.vars)
		free(msh->env.vars);
	if (msh->history.lines)
		free(msh->history.lines);
}
