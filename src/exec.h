#ifndef EXEC_H
#define EXEC_H

# include "debug.h"

# define MAX_SUBSHS_NUM	64 // Maximum number of subshells

typedef struct s_shell	t_shell;

int	exec_ops(t_shell *msh, int *ret_code);
int	close_pipes(t_shell *msh);

#endif
