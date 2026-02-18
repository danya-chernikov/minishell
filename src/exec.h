#ifndef EXEC_H
#define EXEC_H

# include <stdbool.h>
# include <stddef.h>

/* MAX_SUBSHS_NUM - Maximum number of subshells */
# define MAX_SUBSHS_NUM	64
# define MAX_DEPTH		64

typedef struct s_shell		t_shell;
typedef struct s_token		t_token;
typedef struct s_operand	t_operand;

/* exec.c */
int	exec_ops(t_shell *msh, int *ret_code);
int	do_all_expansions_assignments(t_shell *msh, t_token *token);
int	do_expansions_assignments_2nd_lvl_token(t_shell *msh, t_operand *op, size_t *opt_i, int *redir_cnt);

/* exec2.c */
int	close_pipes(t_shell *msh);

#endif
