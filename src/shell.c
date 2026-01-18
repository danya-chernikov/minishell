#include "shell.h"
#include "cmdargs_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
	if (res == COMMON_FAILURE)
		return (COMMON_FAILURE);
	else if (res != COMMON_SUCCESS) // The code we should transfer to the caller
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

	/* Set local and environment variables */
	if (msh_set_local_vars(&msh->env, argv) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	if (msh_set_env_vars(&msh->env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);

	return (COMMON_SUCCESS);
}

/* For each environment variable name
 * and value, we will always allocate
 * memory on the heap, so it will be
 * easier to free everything later
 * in one go */
int	msh_init_param_vars(t_env *env)
{
	t_paramvar	vi;
	size_t		i;
	int			res;
	pid_t		pid;

	vi = PV_HOME;
	// Init ~, $?
	while (vi < PV_PID)
	{
		env->vars[vi].type = PARAM;
		env->vars[vi].f_inherit = false;
		env->vars[vi].value = NULL;
		++vi;
	}
	// Init all the rest variables
	while (vi < PARAM_VARS_NUM)
	{
		env->vars[vi].type = PARAM;
		env->vars[vi].f_readonly = true;
		env->vars[vi].f_inherit = false;
		env->vars[vi].value = NULL;
		++vi;
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

/*
 *
	SL_MSHPID=16,	// MSHPID (can)
	SL_MSHSUBSH,	// MSH_SUBSHELL
	SL_MSHVER,		// MSH_VERSION
	SL_HFSIZE,		// HISTFILESIZE
	SL_HFILE,		// HISTFILE
	SL_HSIZE,		// HISTSIZE
	SL_MSH,			// MSH
	SL_HOSTNAME,	// HOSTNAME
	SL_HOSTTYPE,	// HOSTTYPE
	SL_OSTYPE,		// OSTYPE
	SL_MACHTYPE,	// MACHTYPE
	SL_PS1,			// PS1
	SL_PS2,			// PS2
	SL_PS4,			// PS4
	SL_PPID,		// PPID 
	SL_UID,			// UID
	SL_EUID			// EUID
 * */
int		msh_set_local_vars(t_env *env, char **argv)
{
	t_slocalvar	vi;

	vi = SL_PPID;
	while (vi < SL_MSHSUBSH)
	{
		env->vars[SL_MSHPID].type		= LOCAL;
		env->vars[SL_MSHPID].f_readonly	= true;
		env->vars[SL_MSHPID].f_inherit	= false;
		++vi;
	}

	// PPID
	{
		pid_t	ppid;
		int		res;

		res = ft_getppid(&ppid);
		if (res == -1)
		{
			print_shell_error("getppid()", GETPPID_ERR_MSG);
			return (COMMON_SYS_ERR);
		}
		env->vars[SL_PPID].name			= ft_strdup("PPID");
		env->vars[SL_PPID].value		= ft_itoa((int)ppid);
	}

	// UID
	{
		uid_t	uid;
		int		res;

		res = ft_getuid(&uid);
		if (res == -1)
		{
			print_shell_error("getuid()", GETUID_ERR_MSG);
			return (COMMON_SYS_ERR);
		}	
		env->vars[SL_UID].name			= ft_strdup("UID");
		env->vars[SL_UID].value			= ft_itoa((int)uid);

		// EUID
		env->vars[SL_UID].name			= ft_strdup("EUID");
		env->vars[SL_UID].value			= ft_itoa((int)uid);
	}

	// MSHPID
	// It's calculated on access, i.e. when the shell tries to expand it
	{
		pid_t	pid;
		int		res;

		res = ft_getpid(&pid);
		if (res == -1)
		{
			print_shell_error("getpid()", GETPID_ERR_MSG);
			return (COMMON_SYS_ERR);
		}
		env->vars[SL_MSHPID].name		= ft_strdup("MSHPID");
		env->vars[SL_MSHPID].value		= ft_itoa((int)pid);
	}

	while (vi < PARAM_VARS_NUM + SLOCAL_VARS_NUM)
	{
		env->vars[vi].type			= LOCAL;
		env->vars[vi].f_readonly	= false;
		env->vars[vi].f_inherit		= false;
		++vi;
	}

	// MSH_SUBSHELL	
	env->vars[SL_MSHSUBSH].name		= ft_strdup("MSH_SUBSHELL");
	env->vars[SL_MSHSUBSH].value	= ft_strdup("0");

	// MSH_VERSION	
	env->vars[SL_MSHVER].name		= ft_strdup("MSH_VERSION");
	env->vars[SL_MSHVER].value		= ft_strdup(MSH_VERSION);

	// MSH_HISTFILESIZE
	env->vars[SL_HFSIZE].name		= ft_strdup("HISTFILESIZE");
	env->vars[SL_HFSIZE].value		= ft_itoa((int)DEF_HISTFILESIZE);

	// MSH_HISTFILE
	env->vars[SL_HFILE].name		= ft_strdup("HISTFILE");
	env->vars[SL_HFILE].value		= ft_strdup(DEF_MSH_HIST_PATH);
	
	// MSH_HISTSIZE
	env->vars[SL_HSIZE].name		= ft_strdup("HISTSIZE");
	env->vars[SL_HSIZE].value		= ft_itoa((int)DEF_HISTSIZE);

	// MSH
	env->vars[SL_MSH].name			= ft_strdup("MSH");
	env->vars[SL_MSH].value			= ft_strdup(argv[0]);

	// HOSTNAME	
	int		fd;			// Host file descriptor
	int		gnlerr;		// get_next_line() error
	char	*hostname;

	fd = open(HOSTNAME_PATH, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (COMMON_SYS_ERR);
	}
	hostname = get_next_line(fd, &gnlerr);
	if (!hostname && gnlerr)
	{	
		// Let's consider get_next_line() error as system error
		print_shell_error("get_next_line()", GNL_ERR_MSG);
		gnl_finish(fd); // Obligatory!
		return (COMMON_SYS_ERR);
	}
	if (close(fd) == -1)
	{
		perror("close");
		return (COMMON_SYS_ERR);
	}
	env->vars[SL_HOSTNAME].name		= ft_strdup("HOSTNAME");
	env->vars[SL_HOSTNAME].value	= ft_strdup(hostname);

	// HOSTTYPE	
	env->vars[SL_HOSTTYPE].name		= ft_strdup("HOSTTYPE");
	env->vars[SL_HOSTTYPE].value	= ft_strdup(MSH_ARCH);

	// OSTYPE	
	env->vars[SL_OSTYPE].name		= ft_strdup("OSTYPE");
	env->vars[SL_OSTYPE].value		= ft_strdup(MSH_OSTYPE);
	
	// HOSTMACH	
	char	*hostmach;
	size_t	hm_len;

	hm_len = ft_strlen(MSH_ARCH) + 3 + ft_strlen(MSH_OSTYPE) + 1;
	hostmach = malloc(hm_len * sizeof(char));
	ft_strlcpy(hostmach, MSH_ARCH, hm_len);
	ft_strlcat(hostmach, "-pc-", hm_len);
	ft_strlcat(hostmach, MSH_OSTYPE, hm_len);
	env->vars[SL_HOSTNAME].name		= ft_strdup("HOSTMACH");
	env->vars[SL_HOSTNAME].value	= ft_strdup(hostmach);

	// PS1
	env->vars[SL_PS1].name			= ft_strdup("PS1");
	env->vars[SL_PS1].name			= ft_strdup(DEF_PS1);

	// PS2	
	env->vars[SL_PS2].name			= ft_strdup("PS2");
	env->vars[SL_PS2].value			= ft_strdup(DEF_PS2);

	// PS4
	env->vars[SL_PS4].name			= ft_strdup("PS4");
	env->vars[SL_PS4].value			= ft_strdup(DEF_PS4);

	env->vars_num += SLOCAL_VARS_NUM;

	// Check for malloc() errors	
	vi = SL_PPID;
	while (vi < SL_MSHPID + SLOCAL_VARS_NUM)
	{
		if (!env->vars[vi].name || !env->vars[vi].value)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++vi;
	}

	return (COMMON_SUCCESS);
}

int		msh_set_env_vars(t_env *env)
{
	(void)env;
	return (COMMON_SUCCESS);
}

void	msh_free(t_shell *msh)
{
	msh_free_all_vars(&msh->env);
	if (msh->env.vars)
		free(msh->env.vars);
	if (msh->history.lines)
		free(msh->history.lines);
}
