#ifndef EXPANSION_H
# define EXPANSION_H

/* In order to pass Norminette */
# define CONTINUE	5
# define EXP_RES	0
# define QMASK		1

# include <stddef.h>
# include <stdbool.h>

typedef struct s_shell		t_shell;
typedef struct s_operand	t_operand;
typedef struct s_op_token	t_op_token;
typedef enum e_ind_type		t_ind_type;
typedef struct s_vector		t_vector;

/* exp_assignment.c */
int		exp_process_assignment(t_shell *msh, t_operand *op, t_op_token *op_tok, size_t *opt_i);
int		exp_expand_varname(t_shell *msh, t_operand *op, t_op_token *op_tok, char *var_name);
void	exp_expand_varname_loop(t_shell *msh, char *tok_str, t_vector *vec_pair[]);
void	exp_extract_dlr_varname(char *dlr_varname, char *tok_str, size_t *i);

/* exp_argredir.c */
void	exp_expand_argredir_loop(t_shell *msh, char *tok_str, t_vector *vec_pair[]);

/* exec_preparation.c */
int		exp_divide_op_str_on_tokens(t_operand *op);
int		exp_update_op_tokens_quote_intervals(t_operand *op);

/* exp_common.c */
void	exp_process_double_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type state);
void	exp_process_single_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type state);
void	exp_expand_tilde(t_shell *msh, t_vector *vec_pair[], t_ind_type state);
void	exp_expand_variable(t_shell *msh, t_vector *vec_pair[], char *var_name, t_ind_type state);

/* exec_service.c */
int		exp_vectors_init(t_vector *vec_pair[], size_t cap);
void	exp_vectors_free(t_vector *vec_pair[]);
bool	exp_tilde_found(char *tstr, size_t i, size_t eqsign_ind, t_ind_type state);
bool	exp_token_is_assignment(t_operand *op, t_op_token *op_tok);

/* exec_service2.c */
bool	exp_token_is_redirect(t_op_token *op_tok);
bool	exp_token_is_redir_in(t_op_token *op_tok);
bool	exp_token_is_redir_out(t_op_token *op_tok);
bool	exp_token_is_redir_app(t_op_token *op_tok);
bool	exp_token_is_heredoc(t_op_token *op_tok);

#endif
