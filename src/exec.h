#ifndef EXEC_H
#define EXEC_H

# include <stdbool.h>
# include <stddef.h>

# define MAX_SUBSHS_NUM	64 // Maximum number of subshells

/* In order to pass Norminette */
# define EXEC_CONTINUE	5
# define EXEC_EXP_RES	0
# define EXEC_QMASK		1

typedef struct s_shell		t_shell;
typedef struct s_operand	t_operand;
typedef struct s_op_token	t_op_token;
typedef enum e_ind_type		t_ind_type;
typedef struct s_vector		t_vector;

/* exec.c */
int		exec_ops(t_shell *msh, int *ret_code);
int		exec_process_assignment(t_shell *msh, t_operand *op, t_op_token *op_tok, size_t *opt_i);
int		exec_expand_varname(t_shell *msh, t_operand *op, t_op_token *op_tok, char *var_name);
void	exec_expand_varname_loop(t_shell *msh, char *tok_str, t_vector *vec_pair[]);
void	exec_extract_dlr_varname(char *dlr_varname, char *tok_str, size_t *i);
void	exec_process_double_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type state);
void	exec_process_single_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type state);

/* exec2.c */
int		close_pipes(t_shell *msh);

/* exec_preparation.c */
int		exec_divide_op_str_on_tokens(t_operand *op);
int		exec_update_op_tokens_quote_intervals(t_operand *op);

/* exec_expansions.c */
void	exec_expand_tilde(t_shell *msh, t_vector *vec_pair[], t_ind_type state);
void	exec_expand_variable(t_shell *msh, t_vector *vec_pair[], char *var_name, t_ind_type state);

/* exec_service.c */
int		exec_vectors_init(t_vector *vec_pair[], size_t cap);
bool	exec_tilde_found(char *tstr, size_t i, size_t eqsign_ind, t_ind_type state);
bool	exec_token_is_assignment(t_operand *op, t_op_token *op_tok);

/* exec_service2.c */
bool	exec_token_is_redirect(t_op_token *op_tok);
bool	exec_token_is_redir_in(t_op_token *op_tok);
bool	exec_token_is_redir_out(t_op_token *op_tok);
bool	exec_token_is_redir_app(t_op_token *op_tok);
bool	exec_token_is_heredoc(t_op_token *op_tok);

#endif
