/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 11:13:45 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 11:56:49 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "shell.h"
#include "heredoc.h"
#include "exec.h"
#include "quote.h"
#include "expansion.h"

#include <linux/limits.h>

static int	first_part(t_parser_data *pdata, char *prompt, int *fret);
static int	second_part(t_shell *msh, t_parser_data *pdata, int *fret,
				int *ret_code);

/* If the function returns -1, it means a critical error occurred,
 * and the caller should react by calling exit(EXIT_FAILURE);
 *
 * If it returns 0, a non-critical parser error occurred, and we
 * only need to prompt the user to enter another command in the shell;
 *
 * If it returns 1, everything is fine.
 * The parser_engine() is recursive function! */
int	shell_engine(t_shell *msh, char *prompt, int *ret_code)
{
	t_parser_data	*pdata;
	int				fres;
	int				fret;
	int				try;

	try = 0;
	fres = COMMON_SUCCESS;
	pdata = msh->pd;
	ft_bzero(pdata, sizeof(t_parser_data));
	while (try < 1)
	{
		if (!prompt || ft_strlen(prompt) == 0)
			break ;
		fres = first_part(pdata, prompt, &fret);
		if (fres == BREAK)
			break ;
		fres = second_part(msh, pdata, &fret, ret_code);
		if (fres == BREAK)
			break ;
		++try;
	}
	parser_free(pdata);
	return (fret);
}

static int	first_part(t_parser_data *pdata, char *prompt, int *fret)
{
	*fret = parser_init(pdata, prompt);
	if (*fret != COMMON_SUCCESS)
		return (BREAK);
	*fret = quotes_parser(pdata->prompt, pdata->quotes, &pdata->qpair_cnt);
	if (*fret != COMMON_SUCCESS)
		return (BREAK);
	*fret = comments_parser(pdata);
	*fret = open_par_init(pdata);
	if (*fret == COMMON_FAILURE)
		return (BREAK);
	*fret = close_par_init(pdata);
	if (*fret == COMMON_FAILURE)
		return (BREAK);
	*fret = check_empty_par(pdata);
	if (*fret != COMMON_SUCCESS)
		return (BREAK);
	return (COMMON_SUCCESS);
}

static int	second_part(t_shell *msh, t_parser_data *pdata, int *fret,
				int *ret_code)
{
	*fret = parser_engine(pdata);
	if (*fret != COMMON_SUCCESS)
		return (BREAK);
	*fret = operands_quotes_parser(pdata);
	if (*fret != COMMON_SUCCESS)
		return (BREAK);
	*fret = redirections_parser(pdata);
	if (*fret != COMMON_SUCCESS)
		return (BREAK);
	*fret = read_heredocs(msh);
	if (*fret != COMMON_SUCCESS)
		return (BREAK);
	*fret = exec_ops(msh, ret_code);
	if (*fret != COMMON_SUCCESS)
		return (BREAK);
	return (COMMON_SUCCESS);
}

/* Finds the first comment symbol #
 * that is located outside any quote
 * interval and truncates `prompt`
 * buffer of `s_parser_data` structure
 * eliminating all what goes after this
 * comment symbol.  May be launched only
 * after quote intervals will be parsed
 * calling quotes_parser() */
int	comments_parser(t_parser_data *d)
{
	size_t	pi;

	pi = 0;
	while (pi < ft_strlen(d->prompt))
	{
		if (d->prompt[pi] == '#' && !is_inside_quotes(d, pi))
		{
			if (pi == 0 || (pi > 0 && d->prompt[pi - 1] != '$'))
			{
				d->prompt[pi] = '\0';
				break ;
			}
		}
		++pi;
	}
	return (COMMON_SUCCESS);
}

/* Upates quote indexes for each operand */
int	operands_quotes_parser(t_parser_data *d)
{
	t_operand	*op;
	size_t		ti;

	ti = 0;
	while (ti < d->token_cnt)
	{
		if (d->tokens[ti].type == OPERAND)
		{
			op = d->tokens[ti].op;
			remove_left_spaces(op->name);
			remove_right_spaces(op->name);
			if (!quotes_parser(op->name, op->quotes, &op->qpair_cnt))
				return (COMMON_FAILURE);
		}
		++ti;
	}
	return (COMMON_SUCCESS);
}
