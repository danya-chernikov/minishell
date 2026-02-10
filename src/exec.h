#ifndef EXEC_H
#define EXEC_H

# include <stdbool.h>
# include <stddef.h>

# define MAX_SUBSHS_NUM	64 // Maximum number of subshells

typedef struct s_shell		t_shell;

/* exec.c */
int	exec_ops(t_shell *msh, int *ret_code);

/* exec2.c */
int	close_pipes(t_shell *msh);

#endif
