#ifndef EXEC_H
#define EXEC_H

# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <unistd.h> // Do we need it here?

# include "prompt_parser.h"
# include "shell.h"
# include "debug.h"

# define MAX_SUBSHS_NUM	128 // Maximum number of subshells

int	exec_ops(t_parser_data *d, int *ret_code);
int	close_pipes(t_parser_data *d);

#endif
