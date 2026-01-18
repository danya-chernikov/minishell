#ifndef BUILTIN_H
# define BUILTIN_H

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

# define EXIT_CMD		"exit"
# define CD_CMD			"cd"
# define PWD_CMD		"pwd"
# define ECHO_CMD		"echo"
# define EXPORT_CMD		"export"
# define UNSET_CMD		"unset"
# define ENV_CMD		"env"
# define PRINTENV_CMD	"printenv"
# define SET_CMD		"set"
# define LOCALS_CMD		"locals"
# define HISTORY_CMD	"history"
# define TYPE_CMD		"type"
# define FALSE_CMD		"false"
# define TRUE_CMD		"true"
# define NOP_CMD		":"

#endif
