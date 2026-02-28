/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:41 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:09:05 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_parser.h"
#include "operand.h"

#include "libft.h"
#include "error.h"

#include <stdio.h>

int	copy_hd_delim(t_operand *op, size_t *wi)
{
	size_t	i;
	size_t	op_slen;
	char	*delim;

	i = 0;
	op_slen = ft_strlen(op->name);
	delim = op->redirs[op->red_cnt].hd.delim;
	while (*wi < op_slen)
	{
		if (i >= MAX_HD_DELIM_LEN - 1)
		{
			print_shell_error(NULL, TOO_LONG_HD_DELIM);
			return (COMMON_FAILURE);
		}
		if (redir_word_ends(op, *wi))
			break ;
		delim[i++] = op->name[(*wi)++];
	}
	if (i == 0)
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	delim[i] = '\0';
	return (COMMON_SUCCESS);
}

int	alloc_quotes_buf(t_quote_int **quotes)
{
	*quotes = malloc(MAX_QUOTES_NUM * sizeof(t_quote_int));
	if (!(*quotes))
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

int	get_hd_delimiter(t_operand *op, size_t *wi)
{
	t_quote_int	*quotes;
	size_t		qpair_cnt;
	int			fret;
	t_redir		*redir;

	redir = &op->redirs[op->red_cnt];
	fret = alloc_hd_delim(redir);
	if (fret != COMMON_SUCCESS)
		return (fret);
	fret = copy_hd_delim(op, wi);
	if (fret != COMMON_SUCCESS)
		return (free_redir_buffers(redir), fret);
	redir->hd.f_expand_body = !contains_quote(redir->hd.delim);
	fret = alloc_quotes_buf(&quotes);
	if (fret != COMMON_SUCCESS)
		return (free_redir_buffers(redir), fret);
	fret = quotes_parser(redir->hd.delim, quotes, &qpair_cnt);
	if (fret == COMMON_SUCCESS)
		fret = remove_syntax_quotes(redir->hd.delim, quotes, qpair_cnt);
	free(quotes);
	if (fret != COMMON_SUCCESS)
		return (free_redir_buffers(redir), fret);
	return (COMMON_SUCCESS);
}
