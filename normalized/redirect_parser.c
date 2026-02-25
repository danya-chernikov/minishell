/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 09:33:00 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 21:23:37 by jhvalenc         ###   ########.fr       */
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
 *     i	- auxiliary counter*/
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

static int	process_redir_types(t_operand *op, size_t *op_i)
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

int	redirections_parser(t_parser_data *d)
{
	size_t		ti;
	size_t		op_i;
	t_operand	*op;

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

/* Checks if the redirection limit is reached or if a symbol
 * is misplaced at the very end of the string. */
static int	validate_redir(t_operand *op, size_t *op_i, int size)
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
static int	extr_redir_path(t_operand *op, size_t *wi, char *path, size_t max)
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

/* Handles input redirection '<'. Allocates path memory
 * and updates the operand's redirection array.*/
int	add_redir_in(t_operand *op, size_t *op_i)
{
	size_t	wi;

	if (validate_redir(op, op_i, 1) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op->redirs[op->red_cnt].type = REDIR_IN;
	op->redirs[op->red_cnt].target_fd = STDIN_FILENO;
	wi = *op_i + 1;
	skip_spaces(op->name, &wi);
	if ((op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	op->redirs[op->red_cnt].path = (char *)malloc(PATH_MAX * sizeof(char));
	if (!op->redirs[op->red_cnt].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	if (extr_redir_path(op, &wi, op->redirs[op->red_cnt].path, PATH_MAX) == -1)
		return (COMMON_FAILURE);
	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

/* Handles output redirection '>'. Similar logic to input
 * but targets STDOUT_FILENO.*/
int	add_redir_out(t_operand *op, size_t *op_i)
{
	size_t	wi;

	if (validate_redir(op, op_i, 1) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op->redirs[op->red_cnt].type = REDIR_OUT;
	op->redirs[op->red_cnt].target_fd = STDOUT_FILENO;
	wi = *op_i + 1;
	skip_spaces(op->name, &wi);
	if ((op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	op->redirs[op->red_cnt].path = (char *)malloc(PATH_MAX * sizeof(char));
	if (!op->redirs[op->red_cnt].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	if (extr_redir_path(op, &wi, op->redirs[op->red_cnt].path, PATH_MAX) == -1)
		return (COMMON_FAILURE);
	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

/* Handles append redirection '>>'. Skips two characters
 *and sets type to REDIR_APP.*/
int	add_redir_app(t_operand *op, size_t *op_i)
{
	size_t	wi;

	if (validate_redir(op, op_i, 2) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op->redirs[op->red_cnt].type = REDIR_APP;
	op->redirs[op->red_cnt].target_fd = STDOUT_FILENO;
	wi = *op_i + 2;
	skip_spaces(op->name, &wi);
	if ((op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	op->redirs[op->red_cnt].path = (char *)malloc(PATH_MAX * sizeof(char));
	if (!op->redirs[op->red_cnt].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	if (extr_redir_path(op, &wi, op->redirs[op->red_cnt].path, PATH_MAX) == -1)
		return (COMMON_FAILURE);
	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

/* Handles heredoc '<<'. Prepares the delimiter and
 * allocates memory for the eventual content.*/
int	add_heredoc(t_operand *op, size_t *op_i)
{
	size_t	wi;

	if (validate_redir(op, op_i, 2) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	init_heredoc(op);
	wi = *op_i + 2;
	skip_spaces(op->name, &wi);
	if ((op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	if (get_hd_delimiter(op, &wi) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op->redirs[op->red_cnt].hd.content = malloc(MAX_HD_CONTENT_LEN
			* sizeof(char));
	if (!op->redirs[op->red_cnt].hd.content)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

static void	init_heredoc(t_operand *op)
{
	op->redirs[op->red_cnt].type = REDIR_HEREDOC;
	op->redirs[op->red_cnt].target_fd = STDIN_FILENO;
	op->redirs[op->red_cnt].hd.cnt_len = 0;
}

/* Extracts and cleans the heredoc delimiter.
 * Sets expansion flag based on presence of quotes.*/
int	get_hd_delimiter(t_operand *op, size_t *wi)
{
	int			fret;
	char		*delim;
	size_t		qpair_cnt;
	t_quote_int	*quotes;

	delim = malloc(MAX_HD_DELIM_LEN);
	if (!delim)
		return (perror("malloc"), COMMON_SYS_ERR);
	op->redirs[op->red_cnt].hd.delim = delim;
	if (extract_redir_path(op, wi, delim, MAX_HD_DELIM_LEN) == -1)
		return (COMMON_FAILURE);
	if (contains_quote(delim))
		op->redirs[op->red_cnt].hd.f_expand_body = false;
	else
		op->redirs[op->red_cnt].hd.f_expand_body = true;
	quotes = malloc(MAX_QUOTES_NUM * sizeof (*quotes));
	if (!quotes)
		return (perror("malloc"), COMMON_SYS_ERR);
	fret = quotes_parser(delim, quotes, &qpair_cnt);
	if (fret == COMMON_SUCCESS)
		fret = remove_syntax_quotes(delim, quotes, qpair_cnt);
	free(quotes);
	return (fret);
}
