#include "cmdargs_parser.h"

# define LOGIN_LONG_OPT		"--login"
# define VERBOSE_LONG_OPT	"--verbose"
# define NORC_LONG_OPT		"--norc"
# define HELP_LONG_OPT		"--help"
# define LOGIN_SHORT_OPT	"-l"
# define VERBOSE_SHORT_OPT	"-v"
# define C_SHORT_OPT		"-c"

/*
typedef enum e_shell_mode
{
	NONINT_SCRIPT,	// ./minishell script.sh arg1 arg2
	NONINT_CMD,		// ./minishell -c 'command' OR ./minishell -c "command"
	NONINT_STDIN,	// echo "ls -la" | ./minishell
	INT_LOG,		// ./minishell --login OR ./minishell -l
	INT_NONLOG		// ./minishell
}	t_shell_mode;
 * */
int	cmdargs_parser(t_shell *msh)
{
	int	opt_i;

	opt_i = 1;
	while (opt_i < argc)
	{
		// First go options that not accept any arguments after
		// --help
		if (!ft_strncmp(argv[i], HELP_LONG_OPT, ft_strlen(HELP_LONG_OPT)))
		{
			print_help();
			return (EXIT_SUCCESS_RET); // Stop parsing and call exit(EXIT_SUCCESS) in the caller
		}	
		// -v/--verbose
		if (!ft_strncmp(argv[i], VERBOSE_LONG_OPT, ft_strlen(VERBOSE_LONG_OPT)) ||
			!ft_strncmp(argv[i], VERBOSE_SHORT_OPT, ft_strlen(VERBOSE_SHORT_OPT)))
		{
			msh.opts.f_verbose = true;
		}
		// -l/--login
		if (!ft_strncmp(argv[i], LOGIN_LONG_OPT, ft_strlen(LOGIN_LONG_OPT)) ||
			!ft_strncmp(argv[i], LOGIN_SHORT_OPT, ft_strlen(LOGIN_SHORT_OPT)))
		{
			msh.opts.f_login = true;
		}
		// --norc
		if (!ft_strncmp(argv[i], LOGIN_LONG_OPT, ft_strlen(LOGIN_LONG_OPT)))
		{
			msh.opts.f_norc = true;
		}
		// Then go options which require argument(s) after
		// -c (After -c may go expression in ''/"" or just any number of commands
		if (!ft_strncmp(argv[i], C_SHORT_OPT, ft_strlen(C_SHORT_OPT)))
		{
			msh.opts.f_c = true;
			msh.mode = 
			return (1);	// After locating a -c we always stop parsing of cmd arguments
		}
		else // We gonna try to treat this as a script path leaded by possible arguments
		{

			return (1);	// After locating a script, we always stop parsing
		}
		++opt_i;
	}
	return (1);
}

void	print_help(void)
{
	printf("minishell, version %s-(%s-pc-%s)\n",
		MSH_VERSION, MSH_ARCH, MSH_OSTYPE,
		"Usage:\tminishell [GNU long option] [option] ...\n"
		"\tminishell [GNU long option] [option] script-file ...\n"
		"GNU long options:\n"
		"\t--help\n"
		"\t--login\n"
		"\t--norc\n"
		"\t--verbose\n"
		"\t--version\n"
		"Shell options:\n"
		"\t-clv\n");
}
