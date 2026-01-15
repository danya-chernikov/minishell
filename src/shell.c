#include "shell.h"

/* Initializes the `t_shell` structure, which represents our
 * minishell and stores all its settings.
 * 1. It inherits the arguments of main(): `argc`, `argv`, and `env`;
 * 2. All options are set to false;
 * 3. Memory is allocated for all variables (environment, local,
 *    and shell parameters).
 * 4. Memory is allocated to store the history lines of the
 *    current shell session;
 * 5. Shell parameters are initialized by allocating their names;
 * 6. The mode field will be set later, after all command-line
 *    arguments are parsed;
 * 7. At this stage, `script_path` is set to NULL; it will be
 *    updated during the parsing stage once the exact launch
 *    mode of the shell is determined */
int	msh_init(t_shell *msh, int argc, char **argv, char **env)
{
	int	res;

	// By default, let's think our
	// shell will be interactive
	msh->INT_MODE;

	script_path = NULL;
	c_cmd		= NULL;

	// Assign arguments of main()
	msh->argc	= argc;
	msh->arg	= argv;
	msh-env		= env;

	// Init shell options
	msh->opts.f_login	= false;
	msh->opts.f_verbose	= false;
	msh->opts.f_norc	= false;
	msh->opts.f_c		= false;

	// Allocate environmental variables
	msh->vars = (t_env_var *)malloc(MAX_ENV_VARS_NUM * sizeof(t_env_var));
	if (!msh->vars)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}

	// Init history
	msh->histsize = DEF_HISTSIZE;
	msh->histfilesize = DEF_HISTFILESIZE;
	msh->history = (t_hist_cmd *)malloc(MAX_HIST_LINES_NUM * sizeof(t_hist_cmd));
	if (!msh->history)
	{
		perror("malloc");
		free(msh->vars);
		return (COMMON_SYS_ERR);
	}

	// Initialize shell parameters
	if (init_param_vars(msh->vars) == COMMON_SYS_ERR)
	{
		free(msh->history);
		free(msh->vars);
		return (COMMON_SYS_ERR);
	}

	// Parsing the command-line arguments
	// of our shell. At this stage, we can
	// determine the shell's mode and set
	// some parameter variables
	res = cmdargs_parser(msh);
	if (res == COMMON_SUCCESS)
		return (EXIT_SUCCESS);
	else if (res == COMMON_FAILURE)
		return (COMMON_FAILURE);
	else // The code we should transfer to the caller
		return (res);

	// Here we just determine if print prompt or not
	// if the commands source of our shell is STDIN.
	// Our bash may have 3 command sources
	// 1. bash script.sh			from script
	// 2. bash -c 'commands'		from -c option
	// 3. bash						from STDIN (but we print prompt!)
	//    OR
	//    bash < commands_file		from STDIN (but we do NOT print prompt!)
	//    OR
	//    echo ls | bash | wc -l	from STDIN (but we do NOT print prompt!)
	// So we don't care about stdin stream source of commands is 1 or 2	
	//    echo ls | bash script.sh (`ls` will not be executed! only lines from script.sh are executed)
	//    echo ls | bash -c 'commands' (`ls` will not be executed! only commands after -c will be executed)
	// In all these cases the parent shell who launches bash will handle pipes and redirections!
	if (!isatty(STDIN_FILENO)) // If shell is not connected to any terminal
	{
		if (msh->mode != NONINT_SCRIPT_MODE && msh->mode != NONINT_CMD_MODE)
			msh->mode = NONINT_STDIN; // We do not have to print prompt!
	}

	// Alalyze `env` and set all variables
	set_param_vars(vars);
	set_env_vars(vars);
	set_local_vars(vars);

	return (COMMON_SUCCESS);
}

int	msh_set_vars(t_shell *msh)
{

}

void	msh_free(t_shell *msh)
{
	free_paramvars(msh->vars);
	free(msh->history);
	free(msh->vars);
}
