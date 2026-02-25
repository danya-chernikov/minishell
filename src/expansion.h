/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:50:45 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 23:59:35 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

/* In order to pass Norminette */
# define EXP_RES	0
# define QMASK		1

# include <stddef.h>
# include <stdbool.h>

typedef struct s_shell			t_shell;
typedef struct s_operand		t_operand;
typedef struct s_op_token		t_op_token;
typedef enum e_ind_type			t_ind_type;
typedef struct s_vector			t_vector;
typedef struct s_parser_data	t_parser_data;

typedef struct s_exp_ctx
{
	t_shell		*msh;
	char		*tok_str;
	t_vector	**vec_pair;
	size_t		*i;
	t_ind_type	*state;
}	t_exp_ctx;

typedef struct s_exp_assign_ctx
{
	t_shell		*msh;
	char		*tok_str;
	t_vector	**vec_pair;
	size_t		*i;
	t_ind_type	*state;
	size_t		eqsing_ind;
}	t_exp_assign_ctx;

/* exp_assignment.c */
int		exp_process_assignment(t_shell *msh, t_operand *op,
			t_op_token *op_tok, size_t *opt_i);
int		exp_expand_varname(t_shell *msh, t_operand *op,
			t_op_token *op_tok, char *var_name);
int		set_expanded_var(t_operand *op, char *name, t_vector *vec_pair[]);
void	exp_expand_varname_loop(t_shell *msh, char *tok_str,
			t_vector *vec_pair[]);
void	exp_handle_assign_char(t_exp_assign_ctx *ctx);

/* exp_assignment2.c */
void	exp_handle_assign_dollar(t_exp_assign_ctx *ctx);
void	push_assign_mask(t_vector *vec_pair[], char c, t_ind_type state);

/* exp_argredir.c */
int		exp_process_argredir(t_shell *msh, t_operand *op,
			t_op_token *op_tok, size_t *opt_i);
int		update_redir_path(t_operand *op, t_op_token *tok, char **wc);
int		fill_argv(t_operand *op, char **wc);
void	exp_expand_argredir_loop(t_shell *msh, char *tok_str,
			t_vector *vec_pair[]);
void	exp_handle_char(t_exp_ctx *ctx);

/* exp_argredir2.c */
void	exp_handle_dollar(t_exp_ctx *ctx);
void	push_char_mask(t_vector *vec_pair[], char c, t_ind_type state);

/* exec_preparation.c */
int		exp_divide_op_str_on_tokens(t_operand *op);
int		exp_update_op_tokens_quote_intervals(t_operand *op);

/* exp_common.c */
void	exp_process_double_quote(char *tok_str, size_t *i,
			t_vector *vec_pair[], t_ind_type *state);
void	exp_process_single_quote(char *tok_str, size_t *i,
			t_vector *vec_pair[], t_ind_type *state);
void	exp_expand_tilde(t_shell *msh, t_vector *vec_pair[], t_ind_type state);
void	exp_expand_variable(t_shell *msh, t_vector *vec_pair[],
			char *var_name, t_ind_type state);
bool	exp_extract_dlr_varname(char *dlr_varname,
			const char *tok_str, size_t *i);

/* exp_common.c */
bool	extract_special_var(char *dlr_varname, char c, size_t *i, size_t k);

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
