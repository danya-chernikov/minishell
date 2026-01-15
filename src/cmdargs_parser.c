#include "cmdargs_parser.h"

# define LOGIN_LONG_OPT		"--login"
# define VERBOSE_LONG_OPT	"--verbose"
# define NORC_LONG_OPT		"--norc"
# define HELP_LONG_OPT		"--help"
# define LOGIN_SHORT_OPT	"-l"
# define VERBOSE_SHORT_OPT	"-v"
# define C_SHORT_OPT		"-c"

/* In fact, our command-line argument parser
 * also sets the values of almost all shell
 * parameter variables, except ~ and $?,
 * which will change continuously during
 * the script's execution flow */
int	cmdargs_parser(t_shell *msh)
{
	int	opt_i;

	opt_i = 1;
	while (opt_i < msh->argc)
	{
		// --help
		if (is_help_opt(msh->argv[i]))
		{
			print_help();
			// Stop parsing and invoke
			// exit(EXIT_SUCCESS) in
			// the caller
			break ;
		}

		// --version
		if (is_version_opt(msh->argv[i]))
		{
			print_version();
			break ;
		}

		// -v/--verbose
		if (is_verbose_opt(msh->argv[i]))
			msh->opts.f_verbose = true;

		// -l/--login
		if (is_login_opt(msh->argv[i]))
		{
			msh->opts.f_login = true;
			if (set_argv0_login(msh->argv) = COMMON_SYS_ERR)
				return (COMMON_SYS_ERR);
		}

		// --norc
		if (is_norc_opt(msh->argv[i]))
			msh->opts.f_norc = true;

		// -c
		// After -c may go an expression in '',
		// an expression in "" or just one command
		// without quotes (in this case its arguments
		// and options will not be recognized)
		if (is_c_opt(msh->argv[i]))
		{
			msh->opts.f_c = true;
			msh->mode = NONINT_CMD_MODE;
			// If after -c goes nothing
			if (opt_i == msh->argc - 1)
			{
				print_shell_error(C_SHORT_OPT, C_OPT_ERR_MSG);
				return (SYNTAX_ERR);
			}
			if (!erase_quotes(msh->argv[i + 1]))
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
			msh->c_cmd = argv[i + 1];
			return (COMMON_SUCCESS);
		}
		else
		{
			size_t	i;
			size_t	arg_i;
			size_t	all_argv_len;
			char	*all_argv;

			// If this argument claims to be an option
			if (msh->argv[i][0] == '-')
			{
				print_shell_error(msh->argv[i], INV_OPT_ERR_MSG);
				print_help();
				return (SYNTAX_ERR);
			}
			// We're gonna try to treat this as a script
			// path leaded by possible arguments 
			msh->mode = NONINT_SCRIPT_MODE;
			msh->script = msh->argv[i];

			// Check $0 first (what if before script.sh we had -l/--login options?
			if (!msh->vars[PV_ARGV0].value) // If not exist yet
				msh->vars[PV_ARGV0].value = ft_strdup(argv[0]);

			all_argv_len = 1; // In order to now how much memory allocate to store
			arg_i = 0; // Script's argument index
			while (1 + arg_i < argc && arg_i < SCRIPT_ARGS_NUM)
			{
				msh->vars[PV_ARGV1 + arg_i].value = ft_strdup(argv[1 + arg_i]);
				all_argv_len += ft_strlen(argv1[1 + arg_i]);
				++arg_i;
			}
			// The `arg_i` now contains the number
			// of arguments passed to our script

			// Let's set $# variable
			if (msh->vars[PV_ARGNUM].value)
				free(msh->vars[PV_ARGNUM].value);
			msh->vars[PV_ARGNUM].value = ft_itoa(arg_i);
			
			// Let's set $* variable
			// Just concatenate all arguments passed to the script
			all_argv_len += arg_i - 1; // Count spaces
			++all_argv_len; // Count NULL terminator
			
			all_argv = (char *)malloc(all_argv_len * sizeof(char));
			if (!all_argv)
			{
				perror("malloc");
				return (COMMON_SYS_ERR);
			}

			strlcpy(all_argv, msh->vars[PV_ARGV1], all_argv_len);
			i = 1;
			while (i < arg_i)
			{
				strlcat(all_argv, " ", all_argv_len);
				strlcat(all_argv, msh->vars[PV_ARGV1 + i], all_argv_len);
				++i;
			}
			msh->vars[PV_ALLARGS].value = all_argv;
			
			// After locating a script,
			// we always stop parsing
			break ;
		}
		++opt_i;
	} // while (opt_i < argc)
	return (COMMON_SUCCESS);
}

int	set_argv0_login(t_shell *msh)
{
	char	*new_argv0;
	size_t	new_size;

	new_size = ft_strlen(msh->argv[0]) + 1:
	new_argv0 = (char *)malloc(new_size * sizeof(char));
	if (!new_argv0)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	argv0[0] = '-'; // Mark the login shell
	ft_strlcpy(new_argv0 + sizeof(char), argv0, new_size);
	msh->vars[PV_ARGV0].value = new_argv0;
	return (COMMON_SUCCESS);
}

bool	is_help_opt(char *opt)
{
	return (!ft_strncmp(argv[i], HELP_LONG_OPT, ft_strlen(HELP_LONG_OPT)));
}

bool	is_version_opt(char *opt)
{
	return (!ft_strncmp(argv[i], VERSION_LONG_OPT, ft_strlen(VERSION_LONG_OPT)));
}

bool	is_verbose_opt(char *opt)
{
	if (!ft_strncmp(argv[i], VERBOSE_LONG_OPT, ft_strlen(VERBOSE_LONG_OPT)) ||
		!ft_strncmp(argv[i], VERBOSE_SHORT_OPT, ft_strlen(VERBOSE_SHORT_OPT)))
			return (true);
	return (false);
}

bool	is_login_opt(char *opt)
{
	if (!ft_strncmp(argv[i], LOGIN_LONG_OPT, ft_strlen(LOGIN_LONG_OPT)) ||
		!ft_strncmp(argv[i], LOGIN_SHORT_OPT, ft_strlen(LOGIN_SHORT_OPT)))
			return (true);
	return (false);
}

bool	is_norc_opt(char *opt)
{
	return (!ft_strncmp(argv[i], NORC_LONG_OPT, ft_strlen(NORC_LONG_OPT)));
}

bool	is_s_opt(char *opt)
{
	return (!ft_strncmp(argv[i], C_SHORT_OPT, ft_strlen(C_SHORT_OPT)));
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

void	print_help(void)
{
	printf("%s, version %s-(%s-pc-%s)\n",
		MSH_NAME_LONG, MSH_VERSION, MSH_ARCH, MSH_OSTYPE);
	printf("Usage:\tminishell [GNU long option] [option] ...\n", MSH_NAME_LONG);
	printf("\tminishell [GNU long option] [option] script-file ...\n",
		MSH_NAME_LONG);
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
		MSH_LONG_NAME, MSH_VERSION, MSH_ARCH, MSH_OSTYPE);
	printf("Copyleft (C) 2026 Urduliz 42\n");
	printf("License 42 Educational: <https://42.fr>\n");
	printf("\nThis is free software; you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
}
