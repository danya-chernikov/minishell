/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:05:10 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:05:31 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "heredoc.h"

# define MAX_REDIRS_NUM		256
# define MAX_REDIR_PATH_LEN	4096
# define TOKEN_REDIR_IN		"<"
# define TOKEN_REDIR_OUT	">"
# define TOKEN_REDIR_APP	">>"
# define TOKEN_HEREDOC		"<<"

typedef struct s_operand	t_operand;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APP,
	REDIR_HEREDOC
}	t_redir_type;

/* In case if the redirection is heredoc
 * we're gonna store it's data in `hd`.
 *     tok_ind - the token index of this redirection in
 *				 tokens array. We need it just in case..
 *				 For now we do not even initialize it */
typedef struct s_redir
{
	int				tok_ind;
	t_redir_type	type;
	int				target_fd;
	char			*path;
	t_heredoc		hd;
}	t_redir;

bool	is_redir_in(t_operand *op, size_t op_i);
bool	is_redir_out(t_operand *op, size_t op_i);
bool	is_redir_app(t_operand *op, size_t op_i);
bool	is_heredoc(t_operand *op, size_t op_i);

#endif
