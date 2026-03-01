/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:53:58 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:26:21 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_parser.h"
#include "operand.h"

#include "libft.h"
#include "error.h"

#include <stdio.h>

int	copy_redir_path(t_operand *op, size_t *wi)
{
	size_t	i;
	size_t	op_slen;
	char	*path;

	i = 0;
	op_slen = ft_strlen(op->name);
	path = op->redirs[op->red_cnt].path;
	while (*wi < op_slen)
	{
		if (i >= MAX_REDIR_PATH_LEN - 1)
		{
			print_shell_error(NULL, TOO_LONG_REDIRECT_PATH);
			return (COMMON_FAILURE);
		}
		if (redir_word_ends(op, *wi))
			break ;
		path[i++] = op->name[(*wi)++];
	}
	if (i == 0)
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	path[i] = '\0';
	return (COMMON_SUCCESS);
}

/* data[0] - t_redir_type type;
 * data[1] - int target_fd */
int	add_file_redir(t_operand *op, size_t *op_i, size_t skip, int *data)
{
	t_redir	*redir;
	size_t	wi;
	int		fret;

	fret = init_redir_word(op, &wi, *op_i, skip);
	if (fret != COMMON_SUCCESS)
		return (fret);
	redir = &op->redirs[op->red_cnt];
	redir->type = data[0];
	redir->target_fd = data[1];
	redir->path = NULL;
	fret = alloc_redir_path(redir);
	if (fret != COMMON_SUCCESS)
		return (fret);
	fret = copy_redir_path(op, &wi);
	if (fret != COMMON_SUCCESS)
	{
		free_redir_buffers(redir);
		return (fret);
	}
	*op_i = wi - 1;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

int	redir_word_ends(t_operand *op, size_t wi)
{
	if ((op->name[wi] == ' ' || op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		return (true);
	}
	return (false);
}

void	free_redir_buffers(t_redir *redir)
{
	if (redir->path)
	{
		free(redir->path);
		redir->path = NULL;
	}
	if (redir->hd.content)
	{
		free(redir->hd.content);
		redir->hd.content = NULL;
	}
	if (redir->hd.delim)
	{
		free(redir->hd.delim);
		redir->hd.delim = NULL;
	}
}

int	alloc_hd_delim(t_redir *redir)
{
	redir->hd.delim = (char *)malloc(MAX_HD_DELIM_LEN * sizeof(char));
	if (!redir->hd.delim)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
