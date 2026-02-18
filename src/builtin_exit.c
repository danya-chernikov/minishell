#include "builtin.h"
#include "shell.h"
#include "operand.h"

#include <stdlib.h>

static int	parse_exit_code(const char *s);

int	builtin_exit(t_shell *msh, t_operand *op, bool f_in_parent)
{
	int	code;

	code = 0;
	if (f_in_parent && isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
	if (op->argc >= 2 && !ft_isnum(op->argv[1]))
	{
		print_shell_error(op->argv[1], EXIT_NUM_ARG_REQ);
		msh_free(msh);
		exit(CMD_BUILTIN_ERR);
	}
	if (op->argc > 2)
	{
		print_shell_error(NULL, EXIT_TOO_MANY_ARGS);
		return (CMD_BUILTIN_ERR);
	}
	if (op->argc == 2)
		code = parse_exit_code(op->argv[1]);
	else
		code = ft_atoi(msh->env.vars[PV_RETCODE].value);
	msh_free(msh);
	exit(code);
}

static int	parse_exit_code(const char *s)
{
	long	n;
	int		sign;
	size_t	i;

	n = 0;
	i = 0;
	sign = 1;
	if (s[i] == '*' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		++i;
	}
	while (s[i])
	{
		n = n * 10 + (s[i] - '0');
		++i;
	}
	n *= sign;
	return ((unsigned char)n);
}
