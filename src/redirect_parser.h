/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:01:16 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:26:21 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_PARSER_H
# define REDIRECT_PARSER_H

#include "prompt_parser.h"

typedef struct s_operand	t_operand;

/* redirect_parser.c */
int		redirections_parser(t_parser_data *d);
int		parse_one_redir(t_operand *op, size_t *op_i);
int		check_redir_limit(t_operand *op);
int		init_redir_word(t_operand *op, size_t *wi, size_t op_i, size_t skip);
int		alloc_redir_path(t_redir *redir);

/* redirect_parser2.c */
int		copy_redir_path(t_operand *op, size_t *wi);
int		add_file_redir(t_operand *op, size_t *op_i, size_t skip, int *data);
int		redir_word_ends(t_operand *op, size_t wi);
void	free_redir_buffers(t_redir *redir);
int		alloc_hd_delim(t_redir *redir);

/* redirect_parser3.c */
int		add_redir_in(t_operand *op, size_t *op_i);
int		add_redir_out(t_operand *op, size_t *op_i);
int		add_redir_app(t_operand *op, size_t *op_i);
int		add_heredoc(t_operand *op, size_t *op_i);

/* redirect_parser4.c */
int		copy_hd_delim(t_operand *op, size_t *wi);
int		alloc_quotes_buf(t_quote_int **quotes);
int		get_hd_delimiter(t_operand *op, size_t *wi);


#endif
