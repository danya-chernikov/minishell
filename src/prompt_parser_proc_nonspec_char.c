/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_parser_proc_nonspec_char.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:57:48 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 04:30:53 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt_parser.h"
#include "aux_common.h"

int	process_nonspecial_char(t_parser_data *d, int *opar_ind, size_t prompt_len)
{
	int	fret;

	fret = process_nonspecial_char_prep(d, prompt_len);
	if (fret != COMMON_SUCCESS)
		return (fret);
	if (its_pipe(d->prompt, prompt_len, d->pi))
		return (process_nonspecial_char_handle_pipe(d, opar_ind));
	return (process_nonspecial_char_handle_nonpipe(d, prompt_len));
}

int	process_nonspecial_char_prep(t_parser_data *d, size_t prompt_len)
{
	int	fret;

	if (d->tokens[d->token_cnt - 1].type != PIPE
		&& d->tokens[d->token_cnt - 1].type != OPEN_PAR
		&& d->tokens[d->token_cnt - 1].type != AND
		&& d->tokens[d->token_cnt - 1].type != OR
		&& d->tokens[d->token_cnt - 1].type != NONE)
	{
		print_shell_error(NULL, PARSER_ERR_MSG);
		return (COMMON_FAILURE);
	}
	fret = operand_push(d, prompt_len);
	if (fret != COMMON_SUCCESS)
	{
		print_shell_error(NULL, PARSER_ERR_MSG);
		return (fret);
	}
	token_push(d, OPERAND);
	skip_spaces(d->prompt, &d->pi);
	if (d->pi == prompt_len)
		return (BREAK);
	return (COMMON_SUCCESS);
}

int	process_nonspecial_char_handle_pipe(t_parser_data *d, int *opar_ind)
{
	if (d->tokens[d->token_cnt - 1].type != OPERAND
		&& d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
	{
		print_shell_error(NULL, PARSER_ERR_MSG);
		return (COMMON_FAILURE);
	}
	token_push(d, PIPE);
	*opar_ind = later_goes_open_par(d->prompt, d->pi);
	if (*opar_ind != -1)
	{
		if (handle_open_par(d, *opar_ind) == COMMON_FAILURE)
		{
			print_shell_error(NULL, PARSER_ERR_MSG);
			return (COMMON_FAILURE);
		}
		return (CONTINUE);
	}
	return (COMMON_SUCCESS);
}

int	process_nonspecial_char_handle_nonpipe(t_parser_data *d, size_t prompt_len)
{
	if (d->prompt[d->pi] == ')')
	{
		if (handle_close_par(d) == COMMON_FAILURE)
			return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
		return (BREAK);
	}
	if (its_logical_and(d->prompt, prompt_len, d->pi))
	{
		token_push(d, AND);
		d->pi += 2;
		return (CONTINUE);
	}
	if (its_logical_or(d->prompt, prompt_len, d->pi))
	{
		token_push(d, OR);
		d->pi += 2;
		return (CONTINUE);
	}
	return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
}
