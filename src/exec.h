#ifndef EXEC_H
#define EXEC_H

# include <stdbool.h>

# include "debug.h"

# define MAX_SUBSHS_NUM	64 // Maximum number of subshells

typedef struct s_shell	t_shell;

int		exec_ops(t_shell *msh, int *ret_code);
int		divide_op_str_on_tokens(t_operand *op);
int		update_op_tokens_quote_intervals(t_operand *op);

bool	token_is_assignment(t_operand *op, t_op_token *op_tok);

bool	token_is_redirect(t_op_token *op_tok);
bool	token_is_redir_in(t_op_token *op_tok);
bool	token_is_redir_out(t_op_token *op_tok);
bool	token_is_redir_app(t_op_token *op_tok);
bool	token_is_heredoc(t_op_token *op_tok);

int	close_pipes(t_shell *msh);

#endif
