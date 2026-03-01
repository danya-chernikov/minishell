/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:17 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:24:48 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_parser.h"
#include "operand.h"

#include "libft.h"
#include "error.h"

#include <stdio.h>

/* data[0] - t_redir_type type;
 * data[1] - int target_fd */
int	add_redir_in(t_operand *op, size_t *op_i)
{
	int	data[2];

	data[0] = REDIR_IN;
	data[1] = STDIN_FILENO;
	return (add_file_redir(op, op_i, 1, data));
}

int	add_redir_out(t_operand *op, size_t *op_i)
{
	int	data[2];

	data[0] = REDIR_OUT;
	data[1] = STDOUT_FILENO;
	return (add_file_redir(op, op_i, 1, data));
}

int	add_redir_app(t_operand *op, size_t *op_i)
{
	int	data[2];

	data[0] = REDIR_APP;
	data[1] = STDOUT_FILENO;
	return (add_file_redir(op, op_i, 2, data));
}

int	add_heredoc(t_operand *op, size_t *op_i)
{
	t_redir	*redir;
	size_t	wi;
	int		fret;

	fret = init_redir_word(op, &wi, *op_i, 2);
	if (fret != COMMON_SUCCESS)
		return (fret);
	redir = &op->redirs[op->red_cnt];
	redir->type = REDIR_HEREDOC;
	redir->target_fd = STDIN_FILENO;
	redir->path = NULL;
	redir->hd.delim = NULL;
	redir->hd.content = NULL;
	redir->hd.cnt_len = 0;
	fret = get_hd_delimiter(op, &wi);
	if (fret != COMMON_SUCCESS)
		return (fret);
	redir->hd.content = (char *)malloc(MAX_HD_CONTENT_LEN * sizeof(char));
	if (!redir->hd.content)
		return (free_redir_buffers(redir), perror("malloc"), COMMON_SYS_ERR);
	redir->hd.content[0] = '\0';
	*op_i = wi - 1;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}
