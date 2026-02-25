/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:50:29 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 20:50:29 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

# include "prompt_parser.h"

int	shell_engine(t_shell *msh, char *prompt, int *ret_code);
int	comments_parser(t_parser_data *d);
int	operands_quotes_parser(t_parser_data *d);

int	redirections_parser(t_parser_data *d);
int	add_redir_in(t_operand *op, size_t *op_i);
int	add_redir_out(t_operand *op, size_t *op_i);
int	add_redir_app(t_operand *op, size_t *op_i);
int	add_heredoc(t_operand *op, size_t *op_i);

#endif
