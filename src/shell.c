#include "shell.h"
#include "cmdargs_parser.h"

#include <stdio.h>
#include <stdlib.h>

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
	int		res;

	// By default, let's think our
	// shell will be interactive
	msh->mode = INT_MODE;

	msh->env.vars		= NULL;
	msh->history.lines	= NULL;
	msh->script			= NULL;
	msh->c_cmd			= NULL;

	// Assign arguments of main()
	msh->argc			= argc;
	msh->argv			= argv;
	msh->env.inh_env	= env;

	// Init shell options
	msh->opts.f_login	= false;
	msh->opts.f_verbose	= false;
	msh->opts.f_norc	= false;
	msh->opts.f_c		= false;

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
			msh->mode = NONINT_STDIN_MODE; // We do not have to print prompt!
	}

	// Alalyze `env` and set all variables

	msh_set_env_vars(msh);
	msh_set_local_vars(msh);

	return (COMMON_SUCCESS);
}

/* For each environment variable name and
 * value, we will always allocate memory
 * on the heap, so it will be easier to
 * free everything later in one go */
int	msh_init_param_vars(t_env *env)
{
	size_t	i;
	int		res;
	pid_t	pid;

	i = 0;
	// Init ~, $? and $$
	while (i < 2)
	{
		env->vars[i].type = PARAM;
		env->vars[i].value = NULL;
		++i;
	}
	// Init all the rest variables
	i = 2;
	while (i < PARAM_VARS_NUM)
	{
		env->vars[i].type = PARAM;
		env->vars[i].f_readonly = true;
		env->vars[i].value = NULL;
		++i;
	}
	env->vars[PV_HOME].name		= ft_strdup("~"); // may be changed!
	env->vars[PV_RETCODE].name	= ft_strdup("$?");// may be changed!
	env->vars[PV_PID].name		= ft_strdup("$$");
	env->vars[PV_ARGNUM].name	= ft_strdup("$#");
	env->vars[PV_ALLARGS].name	= ft_strdup("$*");
	env->vars[PV_ARGV0].name	= ft_strdup("$0");
	env->vars[PV_ARGV1].name	= ft_strdup("$1");
	env->vars[PV_ARGV2].name	= ft_strdup("$2");
	env->vars[PV_ARGV3].name	= ft_strdup("$3");
	env->vars[PV_ARGV4].name	= ft_strdup("$4");
	env->vars[PV_ARGV5].name	= ft_strdup("$5");
	env->vars[PV_ARGV6].name	= ft_strdup("$6");
	env->vars[PV_ARGV7].name	= ft_strdup("$7");
	env->vars[PV_ARGV8].name	= ft_strdup("$8");
	env->vars[PV_ARGV9].name	= ft_strdup("$9");

	env->vars_num = PARAM_VARS_NUM;

	// Check for memory errors
	i = 0;
	while (i < PARAM_VARS_NUM)
	{
		if (!env->vars[i].name)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++i;
	}

	// Why not to set $$, $# and $0 immediately...
	// Set $$
	res = ft_getpid(&pid);
	if (res == -1)
	{
		print_shell_error("ft_getpid()", GETPID_ERR_MSG);
		return (COMMON_SYS_ERR);
	}
	env->vars[PV_PID].value = ft_itoa((int)pid);
	// Set $#
	env->vars[PV_ARGNUM].value = ft_strdup("0");

	return (COMMON_SUCCESS);
}


void	msh_free_param_vars(t_env *env)
{
	size_t	i;

	i = 0;
	while (i < PARAM_VARS_NUM)
	{
		if (env->vars[i].name)
			free(env->vars[i].name);
		if (env->vars[i].value)
			free(env->vars[i].value);
		++i;
	}
}

int		msh_set_local_vars(t_shell *msh)
{
	(void)msh;
	return (COMMON_SUCCESS);
}

int		msh_set_env_vars(t_shell *msh)
{
	(void)msh;
	return (COMMON_SUCCESS);
}

void	msh_free(t_shell *msh)
{
	msh_free_param_vars(&msh->env);
	if (msh->env.vars)
		free(msh->env.vars);
	if (msh->history.lines)
		free(msh->history.lines);
}
