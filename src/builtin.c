#include "builtin.h"
#include "aux_common.h"
#include "operand.h"
#include "shell.h"

#include "error.h"
#include "libft.h"

#include <unistd.h>
#include <stdlib.h>

static int	run_builtin_first_part(t_shell *msh, t_operand *op, char *prog);
static int	run_builtin_second_part(t_shell *msh, t_operand *op,
		bool f_in_parent, char *prog);

int	run_builtin(t_shell *msh, t_operand *op, bool f_in_parent)
{
	int		fret;
	char	*prog;

	prog = NULL;
	if (op->argc > 0)
		prog = op->argv[0];
	if (!prog)
	{
		print_shell_error(NULL, OP_ARGV_IS_NULL);
		return (COMMON_FAILURE);
	}
	fret = run_builtin_first_part(msh, op, prog);
	if (fret == CONTINUE)
		fret = run_builtin_second_part(msh, op, f_in_parent, prog);
	return (fret);
}

/* We can use CONTINUE=6 here because we're not processing `exit` command */
static int	run_builtin_first_part(t_shell *msh, t_operand *op, char *prog)
{
	if (strings_equal(prog, ECHO_CMD))
		return (builtin_echo(op));
	if (strings_equal(prog, CD_CMD))
		return (builtin_cd(msh, op));
	if (strings_equal(prog, PWD_CMD))
		return (builtin_pwd());
	if (strings_equal(prog, EXPORT_CMD))
		return (builtin_export(msh, op));
	if (strings_equal(prog, UNSET_CMD))
		return (builtin_unset(msh, op));
	return (CONTINUE);
}

/* Here we do process `exit` command and it may return any value [0, 255] */
static int	run_builtin_second_part(t_shell *msh, t_operand *op,
		bool f_in_parent, char *prog)
{
	if (strings_equal(prog, ENV_CMD) || strings_equal(prog, PRINTENV_CMD))
		return (builtin_env(msh));
	if (strings_equal(prog, EXIT_CMD))
		return (builtin_exit(msh, op, f_in_parent));
	if (strings_equal(prog, TRUE_CMD))
		return (RET_CMD_SUCCESS);
	if (strings_equal(prog, FALSE_CMD))
		return (RET_CMD_FAILURE);
	if (strings_equal(prog, NOP_CMD))
		return (RET_CMD_SUCCESS);
	return (BUILTIN_NOT_FOUND);
}
