/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:52:34 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 20:52:35 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_PARSER_H
# define REDIRECT_PARSER_H

#include "prompt_parser.h"

typedef struct s_operand	t_operand;

int	redirections_parser(t_parser_data *d);
int	add_redir_in(t_operand *op, size_t *op_i);
int	add_redir_out(t_operand *op, size_t *op_i);
int	add_redir_app(t_operand *op, size_t *op_i);
int	add_heredoc(t_operand *op, size_t *op_i);
int	get_hd_delimiter(t_operand *op, size_t *wi);

#endif
