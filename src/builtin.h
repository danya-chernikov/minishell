#ifndef BUILTIN_H
# define BUILTIN_H

#include <stdbool.h>

/* Just built-ins ._. Yeah.. nothing more.
 *
 *     LOCALS_CMD    - our own invention that prints
 *					   all local variables of the
 *					   current minishell session
 *					   created by the user;
 *
 *     NOP_CMD	     - analouge of `nop` assembler
 *					   instruction of almost any CPU
 *
 *     ENV_CMD	     - actually this is not a built-in XD
 *
 *     PRINTENV_CMD - this is also not a built-in 0_0
 *					  But who cares.. But seriously,
 *					  we need to think about priorities
 *					  here. Of course, built-ins should
 *					  always take precedence over any
 *					  other buddy found in the $PATH */

# define EXIT_CMD			"exit"
# define CD_CMD				"cd"
# define PWD_CMD			"pwd"
# define ECHO_CMD			"echo"
# define EXPORT_CMD			"export"
# define UNSET_CMD			"unset"
# define ENV_CMD			"env"
# define PRINTENV_CMD		"printenv"
# define SET_CMD			"set"
# define LOCALS_CMD			"locals"
# define HISTORY_CMD		"history"
# define TYPE_CMD			"type"
# define FALSE_CMD			"false"
# define TRUE_CMD			"true"
# define NOP_CMD			":"

# define RET_CMD_SUCCESS	0
# define RET_CMD_FAILURE	1

# define BUILTIN_NOT_FOUND	1
# define BUILTIN_IN_PARENT	1
# define BUILTIN_IN_CHILD	0

typedef struct s_shell		t_shell;
typedef struct s_operand	t_operand;

/* The thing is: all our builtins should return either
 * RET_CMD_SUCCESS/RET_CMD_FAILURE or sometimes CMD_BUILTIN_ERR */

/* builtin.c */
int	run_builtin(t_shell *msh, t_operand *op, bool f_in_parent);

/* builtin_echo.c */
int	builtin_echo(t_operand *op);

/* builtin_cd.c */
int	builtin_cd(t_shell *msh, t_operand *op);

/* builtin_exit.c */
int	builtin_exit(t_shell *msh, t_operand *op, bool f_in_parent);

/* builtin_export.c */
int	builtin_export(t_shell *msh, t_operand *op);

/* builtin_others.c */
int	builtin_pwd(void);
int	builtin_unset(t_shell *msh, t_operand *op);
int	builtin_env(t_shell *msh);

#endif
