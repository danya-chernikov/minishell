/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:53:07 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 12:53:07 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_parser.h"
#include "operand.h"

#include <stdio.h>
#include <stdlib.h>

/* Preliminary heredoc parser */
int	redirections_parser(t_parser_data *d)
{
	t_operand	*op;
	size_t		ti;
	size_t		op_i;
	int			fret;

	ti = 0;
	while (ti < d->token_cnt)
	{
		if (d->tokens[ti].type == OPERAND)
		{
			op = d->tokens[ti].op;
			op_i = 0;
			while (op_i < ft_strlen(op->name))
			{
				fret = parse_one_redir(op, &op_i);
				if (fret != COMMON_SUCCESS)
					return (fret);
				++op_i;
			}
		}
		++ti;
	}
	return (COMMON_SUCCESS);
}

int	parse_one_redir(t_operand *op, size_t *op_i)
{
	if (is_redir_app(op, *op_i))
		return (add_redir_app(op, op_i));
	if (is_heredoc(op, *op_i))
		return (add_heredoc(op, op_i));
	if (is_redir_out(op, *op_i))
		return (add_redir_out(op, op_i));
	if (is_redir_in(op, *op_i))
		return (add_redir_in(op, op_i));
	return (COMMON_SUCCESS);
}

int	check_redir_limit(t_operand *op)
{
	if (op->red_cnt >= MAX_REDIRS_NUM)
	{
		print_shell_error(NULL, TOO_MANY_REDIRECTS);
		return (COMMON_FAILURE);
	}
	return (COMMON_SUCCESS);
}

int	init_redir_word(t_operand *op, size_t *wi, size_t op_i, size_t skip)
{
	size_t	op_slen;

	if (check_redir_limit(op) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op_slen = ft_strlen(op->name);
	*wi = op_i + skip;
	skip_spaces(op->name, wi);
	if (*wi >= op_slen)
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	if ((op->name[*wi] == '>' || op->name[*wi] == '<')
		&& !is_inside_op_quotes(op, *wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	return (COMMON_SUCCESS);
}

int	alloc_redir_path(t_redir *redir)
{
	redir->path = (char *)malloc(MAX_REDIR_PATH_LEN * sizeof(char));
	if (!redir->path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}
