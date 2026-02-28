#ifndef EXPANSION_H
# define EXPANSION_H

# include "operand.h"

# include <stddef.h>
# include <stdbool.h>

/* In order to pass Norminette */
# define EXP_RES	0
# define QMASK		1

typedef struct s_shell			t_shell;
typedef struct s_op_token		t_op_token;
typedef struct s_vector			t_vector;
typedef struct s_parser_data	t_parser_data;

typedef struct s_exp_ar_loop
{
	char		*tok_str;
	size_t		i;
	t_ind_type	state;
}	t_exp_ar_loop;

typedef struct s_exp_as_loop
{
	char		*tok_str;
	size_t		i;
	t_ind_type	state;
}	t_exp_as_loop;

/* exp_assignment.c */
int		exp_process_assignment(t_shell *msh, t_operand *op,
			t_op_token *op_tok, size_t *opt_i);
int		exp_expand_varname(t_shell *msh, t_operand *op,
			t_op_token *op_tok, char *var_name);
void	exp_expand_varname_loop(t_shell *msh, char *tok_str,
			t_vector *vec_pair[]);
size_t	exp_assign_eqind(char *tok_str);
int		exp_store_var(t_operand *op, char *var_name,
			t_vector *vec_pair[]);

/* exp_assignment2.c */
void	exp_push_assign_char(t_vector *vec_pair[],
			char c, t_ind_type state);
void	exp_assign_handle_dollar(t_shell *msh,
			t_vector *vec_pair[], t_exp_as_loop *ctx);

/* exp_argredir.c */
int		exp_process_argredir(t_shell *msh, t_operand *op,
			t_op_token *op_tok, size_t *opt_i);
int		exp_apply_token_result(t_operand *op, t_op_token *op_tok,
			size_t opt_i, t_vector *vec_pair[]);
int		exp_apply_redir_result(t_operand *op, t_op_token *op_tok,
			t_op_token *prev_tok, t_vector *vec_pair[]);
int		update_redir_path(t_operand *op, t_op_token *op_tok, char **wc_res);
int		append_wc_matches(t_operand *op, char **wc_res);

/* exp_argredir2.c */
int		expand_one_field(t_operand *op, t_vector *vec_pair[],
			size_t start, size_t end);
int		append_split_fields(t_operand *op, t_vector *vec_pair[]);
void	exp_expand_argredir_loop(t_shell *msh, char *tok_str,
			t_vector *vec_pair[]);
void	exp_push_arg_char(t_vector *vec_pair[],
			char c, t_ind_type state);
void	exp_argredir_handle_dollar(t_shell *msh,
			t_vector *vec_pair[], t_exp_ar_loop *ctx);

/* exp_argredir3.c */


/* exp_preparation.c */
int		exp_divide_op_str_on_tokens(t_operand *op);

/* exp_preparation2.c */
int		exp_update_op_tokens_quote_intervals(t_operand *op);

/* exp_common.c */
void	exp_process_double_quote(char *tok_str, size_t *i,
			t_vector *vec_pair[], t_ind_type *state);
void	exp_process_single_quote(char *tok_str, size_t *i,
			t_vector *vec_pair[], t_ind_type *state);
void	exp_expand_tilde(t_shell *msh, t_vector *vec_pair[], t_ind_type state);
bool	exp_extract_dlr_varname(char *dlr_varname, const char *tok_str,
			size_t *i);

/* exp_common2.c */
void	exp_expand_variable(t_shell *msh, t_vector *vec_pair[],
			char *var_name, t_ind_type state);

/* exp_service.c */
int		exp_vectors_init(t_vector *vec_pair[], size_t cap);
void	exp_vectors_free(t_vector *vec_pair[]);
bool	exp_tilde_found_assign(char *tstr, size_t i,
			size_t eqsign_ind, t_ind_type state);
bool	exp_tilde_found_argredir(char *tstr, size_t i, t_ind_type state);
bool	exp_token_is_assignment(t_operand *op, t_op_token *op_tok);

/* exp_service2.c */
bool	exp_token_is_redirect(t_op_token *op_tok);
bool	exp_token_is_redir_in(t_op_token *op_tok);
bool	exp_token_is_redir_out(t_op_token *op_tok);
bool	exp_token_is_redir_app(t_op_token *op_tok);
bool	exp_token_is_heredoc(t_op_token *op_tok);

/* exp_service3.c */
int		exp_alloc_argv(t_operand *op);
void	exp_free_argv(t_operand	*op);
void	exp_free_all_ops_argv(t_parser_data *d);
void	exp_free_op_tokens(t_operand *op);

#endif
