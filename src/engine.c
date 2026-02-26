/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 01:40:12 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/26 01:18:02 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "shell.h"
#include "heredoc.h"
#include "exec.h"
#include "quote.h"
#include "expansion.h"
#include "redirect_parser.h"

#include <linux/limits.h>

/* If the function returns -1, it means a critical error occurred,
 * and the caller should react by calling exit(EXIT_FAILURE);
 *
 * If it returns 0, a non-critical parser error occurred, and we
 * only need to prompt the user to enter another command in the shell;
 *
 * If it returns 1, everything is fine.
 * The parser_engine() is recursive function!*/
int	shell_engine(t_shell *msh, char *prompt, int *ret_code)
{
	t_parser_data	*pdata;
	int				fres;

	fres = COMMON_SUCCESS;
	pdata = msh->pd;
	ft_bzero(pdata, sizeof(t_parser_data));
	if (!prompt || ft_strlen(prompt) == 0)
	{
		parser_free(pdata);
		return (fres);
	}
	fres = parser_init(pdata, prompt);
	if (fres == COMMON_SUCCESS)
		fres = quotes_parser(pdata->prompt, pdata->quotes, &pdata->qpair_cnt);
	if (fres == COMMON_SUCCESS)
		fres = comments_parser(pdata);
	if (fres == COMMON_SUCCESS)
		fres = run_parser(msh, pdata, ret_code);
	parser_free(pdata);
	return (fres);
}

/* Executes the core parsing and execution sequence.
 * It ensures that each step (parenthesis check, engine,
 * quote handling, redirections, and heredocs) is successful
 * before proceeding to the final execution of operations.
 * Returns COMMON_SUCCESS (1) if the entire sequence completes,
 * or the specific error code from the failed step. */
int	run_parser(t_shell *msh, t_parser_data *pdata, int *ret_code)
{
	int	fres;

	fres = check_empty_par(pdata);
	if (fres == COMMON_SUCCESS)
		fres = parser_engine(pdata);
	if (fres == COMMON_SUCCESS)
		fres = operands_quotes_parser(pdata);
	if (fres == COMMON_SUCCESS)
		fres = redirections_parser(pdata);
	if (fres == COMMON_SUCCESS)
		fres = read_heredocs(msh);
	if (fres == COMMON_SUCCESS)
		fres = exec_ops(msh, ret_code);
	return (fres);
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
