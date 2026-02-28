/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_parser_proc_spec_char2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:28:10 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 16:39:23 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt_parser.h"
#include "aux_common.h"

int	process_pipe_and_logical_operators(t_parser_data *d, size_t prompt_len)
{
	int	fret;

	if (its_pipe(d->prompt, prompt_len, d->pi))
	{
		fret = process_special_char_handle_pipe(d);
		if (fret != COMMON_SUCCESS)
			return (fret);
	}
	else if (its_logical_and(d->prompt, prompt_len, d->pi))
	{
		fret = process_special_char_handle_and(d);
		if (fret != COMMON_SUCCESS)
			return (fret);
		return (CONTINUE);
	}
	else if (its_logical_or(d->prompt, prompt_len, d->pi))
	{
		fret = process_special_char_handle_or(d);
		if (fret != COMMON_SUCCESS)
			return (fret);
		return (CONTINUE);
	}
	else
		return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
	return (COMMON_SUCCESS);
}

int	process_special_char_handle_pipe(t_parser_data *d)
{
	if (d->tokens[d->token_cnt - 1].type != CLOSE_PAR
		&& d->tokens[d->token_cnt - 1].type != OPERAND)
	{
		print_shell_error(NULL, PARSER_ERR_MSG);
		return (COMMON_FAILURE);
	}
	token_push(d, PIPE);
	return (COMMON_SUCCESS);
}

int	process_special_char_handle_and(t_parser_data *d)
{
	if (d->tokens[d->token_cnt - 1].type != OPERAND
		&& d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
	{
		print_shell_error(NULL, PARSER_ERR_MSG);
		return (COMMON_FAILURE);
	}
	token_push(d, AND);
	d->pi += 2;
	return (COMMON_SUCCESS);
}

int	process_special_char_handle_or(t_parser_data *d)
{
	if (d->tokens[d->token_cnt - 1].type != OPERAND
		&& d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
	{
		print_shell_error(NULL, PARSER_ERR_MSG);
		return (COMMON_FAILURE);
	}
	token_push(d, OR);
	d->pi += 2;
	return (COMMON_SUCCESS);
}
