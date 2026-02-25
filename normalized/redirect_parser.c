/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 09:33:00 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/26 00:21:13 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_parser.h"
#include "operand.h"

#include <stdio.h>
#include <stdlib.h>

/* Preliminary heredoc parser.
 *     ti	- token index;
 *     op_i	- operand's content index;
 *     wi   - redirection's operand index
 *			  (a 'word' that goes after a
 *			  redirection or heredoc);
 *     i	- auxiliary counter */
int	redirections_parser(t_parser_data *d)
{
	t_operand	*op;
	size_t		ti;
	size_t		op_i;

	ti = 0;
	while (ti < d->token_cnt)
	{
		if (d->tokens[ti].type == OPERAND)
		{
			op = d->tokens[ti].op;
			op_i = 0;
			while (op_i < ft_strlen(op->name))
			{
				if (process_redir_types(op, &op_i) == -1)
					return (COMMON_FAILURE);
				++op_i;
			}
		}
		++ti;
	}
	return (COMMON_SUCCESS);
}

int	process_redir_types(t_operand *op, size_t *op_i)
{
	if (is_redir_app(op, *op_i))
		return (add_redir_app(op, op_i));
	if (is_heredoc(op, *op_i))
		return (add_heredoc(op, *op_i));
	if (is_redir_out(op, *op_i))
		return (add_redir_out(op, *op_i));
	if (is_redir_in(op, *op_i))
		return (add_redir_in(op, *op_i));
	return (0);
}

/* Checks if the redirection limit is reached or if a symbol
 * is misplaced at the very end of the string. */
int	validate_redir(t_operand *op, size_t *op_i, int size)
{
	if (op->red_cnt == MAX_REDIRS_NUM - 1)
	{
		print_shell_error(NULL, TOO_MANY_REDIRECTS);
		return (COMMON_FAILURE);
	}
	if (*op_i == ft_strlen(op->name) - size)
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	return (COMMON_SUCCESS);
}

/* Extracts the path or delimiter string, stopping at spaces 
 * or new redirection symbols outside of quotes.*/
int	extr_redir_path(t_operand *op, size_t *wi, char *path, size_t max)
{
	size_t	i;
	size_t	op_slen;

	i = 0;
	op_slen = ft_strlen(op->name);
	while (*wi < op_slen)
	{
		if (i >= max - 1)
		{
			if (max == PATH_MAX)
				print_shell_error(NULL, TOO_LONG_REDIRECT_PATH);
			else
				print_shell_error(NULL, TOO_LONG_HD_DELIM);
			return (COMMON_FAILURE);
		}
		if ((op->name[*wi] == ' ' || op->name[*wi] == '>'
				|| op->name[*wi] == '<') && !is_inside_op_quotes(op, *wi))
			break ;
		path[i++] = op->name[(*wi)++];
	}
	path[i] = '\0';
	return (COMMON_SUCCESS);
}

void	init_heredoc(t_operand *op)
{
	op->redirs[op->red_cnt].type = REDIR_HEREDOC;
	op->redirs[op->red_cnt].target_fd = STDIN_FILENO;
	op->redirs[op->red_cnt].hd.cnt_len = 0;
}
