#include "cmdargs_parser.h"

#include <stdio.h>
#include <stdlib.h>

# define LOGIN_LONG_OPT		"--login"
# define VERBOSE_LONG_OPT	"--verbose"
# define NORC_LONG_OPT		"--norc"
# define HELP_LONG_OPT		"--help"
# define LOGIN_SHORT_OPT	"-l"
# define VERBOSE_SHORT_OPT	"-v"
# define C_SHORT_OPT		"-c"

/* In fact, our command-line argument
 * parser also sets the values of almost
 * all shell parameter variables, except
 * ~ and $?, which will change continuously
 * during the script's execution flow */
int	cmdargs_parser(t_shell *msh)
{
	int	opt_i;

	// Set $0
	get_last_path_comp(msh->argv[0]);
	msh->env.vars[PV_ARGV0].value = ft_strdup(msh->argv[0]);

	opt_i = 1;
	while (opt_i < msh->argc)
	{
		// --help
		if (is_help_opt(msh->argv[opt_i]))
		{
			print_help();
			// Stop parsing and invoke
			// exit(EXIT_SUCCESS) in
			// the caller
			break ;
		}

		// --version
		else if (is_version_opt(msh->argv[opt_i]))
		{
			print_version();
			break ;
		}

		// -v/--verbose
		else if (is_verbose_opt(msh->argv[opt_i]))
		{
			msh->opts.f_verbose = true;
		}

		// -l/--login
		else if (is_login_opt(msh->argv[opt_i]))
		{
			msh->opts.f_login = true;
			if (set_argv0_login(msh) == COMMON_SYS_ERR)
				return (COMMON_SYS_ERR);
		}

		// --norc
		else if (is_norc_opt(msh->argv[opt_i]))
			msh->opts.f_norc = true;

		// -c
		// After -c may go an expression in '',
		// an expression in "" or just one command
		// without quotes (in this case its arguments
		// and options will not be recognized)
		else if (is_c_opt(msh->argv[opt_i]))
		{
			return (handle_c_opt(msh, opt_i));
		}
		else
		{
			// Check for invalid option
			// If this argument claims to be an option
			if (msh->argv[opt_i][0] == '-')
			{
				print_shell_error(msh->argv[opt_i], INV_OPT_ERR_MSG);
				print_help();
				return (SYNTAX_ERR);
			}		
			return (handle_script(msh, opt_i));
			// After locating a script,
			// we always stop parsing
		}
		++opt_i;
	} // while (opt_i < argc)
	return (COMMON_SUCCESS);
}

int	handle_c_opt(t_shell *msh, int opt_i)
{
	msh->opts.f_c = true;
	msh->mode = NONINT_CMD_MODE;
	// If after -c goes nothing
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
	// If after erasing quotes nothing left,
	// e.g. when user run
	//     $ bash -c ''
	// We simply pass an empty string to
	// the engine, and it, in turn, does
	// nothing when it sees an empty line
	
	// After locating a -c we always
	// stop parsing cmd arguments
	msh->c_cmd = msh->argv[opt_i + 1];
	return (COMMON_SUCCESS);
}

int	handle_script(t_shell *msh, int opt_i)
{
	int		arg_i;
	size_t	allargv_len;

	// We're gonna try to treat this as a script
	// path leaded by possible arguments 
	msh->mode = NONINT_SCRIPT_MODE;
	msh->script = msh->argv[opt_i];

	if (set_script_args(msh, opt_i, &allargv_len, &arg_i) == SYNTAX_ERR)
		return (SYNTAX_ERR);

	// Let's set $# variable
	if (msh->env.vars[PV_ARGNUM].value)
		free(msh->env.vars[PV_ARGNUM].value);
	msh->env.vars[PV_ARGNUM].value = ft_itoa(arg_i);
	
	if (set_allargs_var(msh, allargv_len, arg_i) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);

	return (COMMON_SUCCESS);
}

int	set_script_args(t_shell *msh, int opt_i, size_t *allargv_len, int *arg_i)
{
	// Check $0 first (what if before script.sh we had -l/--login options?
	if (!msh->env.vars[PV_ARGV0].value) // If not exist yet
		msh->env.vars[PV_ARGV0].value = ft_strdup(msh->argv[opt_i]);

	*allargv_len = 0; // In order to know how much memory allocate to store
	*arg_i = 1; // Script's argument index
	while (opt_i + *arg_i < msh->argc && *arg_i - 1 < SCRIPT_ARGS_NUM)
	{
		if (!erase_quotes(msh->argv[opt_i + *arg_i]))
		{
			print_shell_error(C_SHORT_OPT, SYNTAX_ERR_MSG);
			return (SYNTAX_ERR);
		}
		msh->env.vars[PV_ARGV0 + *arg_i].value = ft_strdup(msh->argv[opt_i + *arg_i]);
		*allargv_len += ft_strlen(msh->argv[opt_i + *arg_i]);
		++(*arg_i);
	}
	--(*arg_i);
	// The `arg_i` now contains the number
	// of arguments passed to our script
	return (COMMON_SUCCESS);
}

int	set_allargs_var(t_shell *msh, size_t allargv_len, int arg_i)
{
	char	*allargv;
	int		i;

	if (arg_i > 0) // If the script had any arguments
	{
		// Let's set $* variable
		// Just concatenate all arguments passed to the script
		allargv_len += arg_i - 1; // Count spaces
		++allargv_len; // Count the NULL terminator
		
		allargv = malloc(allargv_len * sizeof(char));
		if (!allargv)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}

		ft_strlcpy(allargv, msh->env.vars[PV_ARGV1].value, allargv_len);
		i = 1;
		while (i < arg_i)
		{
			ft_strlcat(allargv, " ", allargv_len);
			ft_strlcat(allargv, msh->env.vars[PV_ARGV1 + i].value, allargv_len);
			++i;
		}
		msh->env.vars[PV_ALLARGS].value = allargv;
	}
	return (COMMON_SUCCESS);
}

int	set_argv0_login(t_shell *msh)
{
	char	*new_argv0;
	size_t	new_size;

	get_last_path_comp(msh->argv[0]);
	new_size = ft_strlen(msh->argv[0]) + 1;
	new_argv0 = malloc(new_size * sizeof(char));
	if (!new_argv0)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	new_argv0[0] = '-'; // Mark the login shell
	ft_strlcpy(new_argv0 + sizeof(char), msh->argv[0], new_size);
	if (msh->env.vars[PV_ARGV0].value)
		free(msh->env.vars[PV_ARGV0].value);
	msh->env.vars[PV_ARGV0].value = new_argv0;
	return (COMMON_SUCCESS);
}

/* Removes possible quotes
 * surrounding `str`. If
 * it's impossible to remove
 * the quotes correctly, it
 * reports an error.
 * For example:
 *     bash -c 'ls -> error
 *     bash -c ls" -> error
 *     bash -c 'ls" -> error
 *     bash -c 'ls' -> ok
 *     bash -c "ls" -> ok */
int	erase_quotes(char *str)
{
	size_t	i;

	if ((str[0] == '\'' && str[ft_strlen(str) - 1] != '\'') ||
		(str[0] == '"' && str[ft_strlen(str) - 1] != '"'))
	{
		return (COMMON_FAILURE);
	}
	if ((str[ft_strlen(str) - 1] == '\'' && str[0] != '\'') ||
		(str[ft_strlen(str) - 1] == '"' && str[0] != '"'))
	{
		return (COMMON_FAILURE);
	}
	if (str[0] == '\'' || str[0] == '"')
	{
		str[ft_strlen(str) - 1] = '\0';
		i = 0;
		while (i < ft_strlen(str) - 1)
		{
			str[i] = str[i + 1];
			++i;
		}
	}
	return (COMMON_SUCCESS);
}

/* Eliminates all except the last path component.
 * For example:
 *     path = "/home/dchernik/Downloads/circle03/minishell/src/minishell"
 *     get_last_path_comp(path);
 *     path = "minishell" */
void	get_last_path_comp(char *path) // Give it better name
{
	int	i;
	int	j;
	int	slash_ind;
	int	path_len;

	i = 0;
	slash_ind = -1;
	path_len = (int)ft_strlen(path);
	while (i < path_len)
	{
		if (path[i] == '/')
			slash_ind = i;
		++i;
	}
	if (slash_ind == -1)
		return ;
	j = 0;
	i = slash_ind + 1;
	while (i < path_len)
	{
		path[j] = path[i];
		++i;
		++j;
	}
	path[j] = '\0';
}

bool	is_help_opt(char *opt)
{
	if (!ft_strncmp(opt, HELP_LONG_OPT, ft_strlen(HELP_LONG_OPT)))
		return (true);
	return (false);
}

bool	is_version_opt(char *opt)
{
	if (!ft_strncmp(opt, VERSION_LONG_OPT, ft_strlen(VERSION_LONG_OPT)))
		return (true);
	return (false);
}

bool	is_verbose_opt(char *opt)
{
	if (!ft_strncmp(opt, VERBOSE_LONG_OPT, ft_strlen(VERBOSE_LONG_OPT)) ||
		!ft_strncmp(opt, VERBOSE_SHORT_OPT, ft_strlen(VERBOSE_SHORT_OPT)))
			return (true);
	return (false);
}

bool	is_login_opt(char *opt)
{
	if (!ft_strncmp(opt, LOGIN_LONG_OPT, ft_strlen(LOGIN_LONG_OPT)) ||
		!ft_strncmp(opt, LOGIN_SHORT_OPT, ft_strlen(LOGIN_SHORT_OPT)))
			return (true);
	return (false);
}

bool	is_norc_opt(char *opt)
{
	if (!ft_strncmp(opt, NORC_LONG_OPT, ft_strlen(NORC_LONG_OPT)))
		return (true);
	return (false);
}

bool	is_c_opt(char *opt)
{
	if (!ft_strncmp(opt, C_SHORT_OPT, ft_strlen(C_SHORT_OPT)))
		return (true);
	return (false);
}

void	print_help(void)
{
	printf("%s, version %s-(%s-pc-%s)\n",
		MSH_NAME_LONG, MSH_VERSION, MSH_ARCH, MSH_OSTYPE);
	printf("Usage:\t%s [GNU long option] [option] ...\n", MSH_NAME_LONG);
	printf("\t%s [GNU long option] [option] script-file ...\n", MSH_NAME_LONG);
	printf("GNU long options:\n"
		"\t--help\n"
		"\t--login\n"
		"\t--norc\n"
		"\t--verbose\n"
		"\t--version\n"
		"Shell options:\n"
		"\t-clv\n");
}

void	print_version(void)
{
	printf("%s, version %s (%s-pc-%s)",
		MSH_NAME_LONG, MSH_VERSION, MSH_ARCH, MSH_OSTYPE);
	printf("Copyleft (C) 2026 Urduliz 42\n");
	printf("License 42 Educational: <https://42.fr>\n");
	printf("\nThis is free software; you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
}
