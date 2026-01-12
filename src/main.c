#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "libft.h"

#include "shell.h"
#include "engine.h"
#include "builtin.h" // for exit

/*# define LOGIN_LONG_OPT		"--login"
# define VERBOSE_LONG_OPT	"--verbose"
# define NORC_LONG_OPT		"--norc"
# define HELP_LONG_OPT		"--help"
# define LOGIN_SHORT_OPT	"-l"
# define VERBOSE_SHORT_OPT	"-v"
# define C_SHORT_OPT		"-c"*/

/*
typedef struct e_shell
{
	t_shell_mode	mode;
	t_options		opts;
	t_configs		configs;
	t_env_var		vars;
	char			**argv;
	char			**env;
	int				histsize;
	int				histfilesize;
	char			*script_path;
}	t_shell;
 * */

int	main(int argc, char **argv, char **env)
{
	t_shell	msh;
	char	prompt[PROMPT_INV_LEN];
	char	*rline_buf;
	int		ret_code; // Shells' return code
	int		opt_i;

	msh_init(&msh);
	ret_code = EXIT_SUCCESS;
	opt_i = 1;
	while (opt_i < argc)
	{
		if (!ft_strncmp(argv[i], HELP_LONG_OPT, ft_strlen(HELP_LONG_OPT)))
		{
			print_help();
			exit(ret_code);
		}	
		if (!ft_strncmp(argv[i], VERBOSE_LONG_OPT, ft_strlen(VERBOSE_LONG_OPT)) ||
			!ft_strncmp(argv[i], VERBOSE_SHORT_OPT, ft_strlen(VERBOSE_SHORT_OPT)))
		{
			msh.opts.f_verbose = true;
		}
		if (!ft_strncmp(argv[i], LOGIN_LONG_OPT, ft_strlen(LOGIN_LONG_OPT)) ||
			!ft_strncmp(argv[i], LOGIN_SHORT_OPT, ft_strlen(LOGIN_SHORT_OPT)))
		{
			msh.opts.f_login = true;
		}
		if (!ft_strncmp(argv[i], LOGIN_LONG_OPT, ft_strlen(LOGIN_LONG_OPT)) ||
			!ft_strncmp(argv[i], LOGIN_SHORT_OPT, ft_strlen(LOGIN_SHORT_OPT)))
		{
			msh.opts.f_login = true;
		}
		++opt_i;
	}

	rline_buf = NULL;
	strncpy(prompt, "dchernik@c3r3s6: ", PROMPT_INV_LEN);
	while (1)
	{
		rline_buf = readline(prompt);
		if (ft_strlen(rline_buf) == 0)
		{
			free(rline_buf);
			rline_buf = NULL;
			continue;
		}
		add_history(rline_buf);
		if (!strncmp(rline_buf, EXIT_CMD, ft_strlen(EXIT_CMD)) &&
			ft_strlen(rline_buf) == ft_strlen(EXIT_CMD))
		{
			free(rline_buf);
			rline_buf = NULL;
			break;
		}
		if (shell_engine(rline_buf, &ret_code) == -1) // Critial system error occured
			exit(EXIT_FAILURE);
		// In case if non-critial parser error occured
		// we just free `rline_buf` and prompt user again
		free(rline_buf);
		rline_buf = NULL;
	}
	return (0);
}
