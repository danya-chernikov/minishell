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

	script_path = NULL;

	// Assign arguments of main()
	msh->argc	= argc;
	msh->arg	= argv;
	msh-env		= env;

	// Init shell options
	msh->opts.f_login = false;
	msh->opts.f_verbose = false;
	msh->opts.f_norc = false;
	msh->opts.f_c = false;

	// Allocate environmental variables
	msh->vars = (t_env_var *)malloc(MAX_ENV_VARS_NUM * sizeof(t_env_var));
	if (!msh->vars)
	{
		perror("malloc");
		return (0);
	}
	// Init history
	msh->histsize = DEF_HISTSIZE;
	msh->histfilesize = DEF_HISTFILESIZE;
	msh->history = (t_hist_cmd *)malloc(MAX_HIST_LINES_NUM * sizeof(t_hist_cmd));
	if (!msh->history)
	{
		perror("malloc");
		free(msh->vars);
		return (0);
	}
	// Create shell parameters
	if (!init_paramvars(msh->vars))
	{
		free(msh->history);
		free(msh->vars);
		return (0);
	}

	// After this shell options may be cahanged
	res = cmdargs_parser(&msh);
	if (res == 0) // Error
		return (0);
	else if (res == EXIT_SUCCESS_RET) // We should call exit(EXIT_SUCCESS)
		return (EXIT_SUCCESS_RET);

	/*
	NONINT_SCRIPT,	// ./minishell script.sh arg1 arg2
	NONINT_CMD,		// ./minishell -c 'command' OR ./minishell -c "command"
	NONINT_STDIN,	// echo "ls -la" | ./minishell
	INT_LOG,		// ./minishell --login OR ./minishell -l
	INT_NONLOG		// ./minishell
	*/

	// Here we just determine if print prompt or not
	// if the commands source of our shell is STDIN
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
		if (msh->mode != NONINT_SCRIPT && msh->mode != NONINT_CMD) // 
			msh->mode = NONINT_STDIN; // We do not have to print prompt!
	}
	else // Let's check if user launched smth like "./minishell < file"
	{
		// Try non-blocking reading from stdin
		// If there is no any data accessible
		// We consider bash was run in any other mode than NONINT_STDIN
	}

	// Alalyze `env` and set all variables
	set_param_vars(vars);
	set_env_vars(vars);
	set_local_vars(vars);

	return (1);
}

int	msh_set_vars(t_shell*msh)
{

}

void	msh_free(t_shell *msh)
{
	free_paramvars(msh->vars);
	free(msh->history);
	free(msh->vars);
}
