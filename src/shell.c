#include "shell.h"
#include "cmdargs_parser.h"
#include "prompt_parser.h"
#include "expansion.h"

#include "debug.h"
#include "error.h"
#include "libft.h"

#include <stdlib.h>

static int	prelim_struct_init(t_shell *msh, int argc, char **argv);

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
	int	fres;

	if (prelim_struct_init(msh, argc, argv) != COMMON_SUCCESS)
		return (COMMON_SYS_ERR);

	// Allocate environmental variables
	if (env_init(&msh->env, env) == COMMON_SYS_ERR)
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
	fres = cmdargs_parser(msh);
	if (fres != COMMON_SUCCESS) // The code we should transfer to the caller
		return (fres);

	if (!isatty(STDIN_FILENO)) // If shell is not connected to any terminal
	{
		if (msh->mode != NONINT_SCRIPT_MODE && msh->mode != NONINT_CMD_MODE)
			msh->mode = NONINT_STDIN_MODE; // We do not have to print prompt!
	}

	/* Set local and environment variables */
	if (msh_set_local_vars(&msh->env, argv) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	fres = msh_set_env_vars(&msh->env);
	if (fres != COMMON_SUCCESS)
		return (fres);

#if DEBUG == 1
	printf("Local variables:\n");
	env_print_locals(&msh->env);
	printf("\n");
	printf("Environment variables:\n");
	env_print_env(&msh->env);
#endif

	// Read configs
	if (msh_load_configs(msh) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
		
	return (COMMON_SUCCESS);
}

void	msh_free(t_shell *msh)
{
	exp_free_all_ops_argv(msh->pd); // TO CORRECT LEAK
	parser_free(msh->pd); // TO CORRECT LEAK
	if (msh->pd)
		free(msh->pd);
	if (msh->prompt_inv)
		free(msh->prompt_inv);
	env_free(&msh->env);
	history_free(&msh->history);
	configs_free(&msh->configs);
}

static int	prelim_struct_init(t_shell *msh, int argc, char **argv)
{
	// By default, let's think our
	// shell will be interactive
	msh->mode = INT_MODE;
	msh->env.vars = NULL;
	msh->history.lines = NULL;
	msh->prompt_inv = NULL;
	msh->script = NULL;
	msh->c_cmd = NULL;
	// Assign arguments of main()
	msh->argc = argc;
	msh->argv = argv;
	// Init shell options
	msh->opts.f_login = false;
	msh->opts.f_verbose = false;
	msh->opts.f_norc = false;
	msh->opts.f_c = false;
	// Init configs
	configs_init(&msh->configs);
	// Init t_parser_data
	msh->pd = (t_parser_data *)ft_calloc(1, sizeof(t_parser_data));
	if (!msh->pd)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
