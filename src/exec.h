#ifndef EXEC_H
#define EXEC_H

# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <unistd.h> // Do we need it here?
# include "builtin.h"
# include "path_finder.h"
# include "env.h"
# include "libft.h"
# include "prompt_parser.h"
# include "shell.h"
# include "debug.h"
# include <errno.h>
# include <string.h>

typedef struct e_shell	t_shell;

# define MAX_SUBSHS_NUM	128 // Maximum number of subshells

//int	exec_ops(t_parser_data *d, int *ret_code);
int	close_pipes(t_parser_data *d);
int	exec_ops(t_parser_data *d, t_shell *msh, int *ret_code);

#endif
