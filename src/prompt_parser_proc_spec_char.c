/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_parser_proc_spec_char.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:56:39 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 16:38:32 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt_parser.h"
#include "aux_common.h"

int	process_special_char(t_parser_data *d, size_t prompt_len)
{
	int	fret;

	fret = process_special_char_check_pars(d);
	if (fret != COMMON_SUCCESS)
		return (fret);
	fret = process_pipe_and_logical_operators(d, prompt_len);
	if (fret != COMMON_SUCCESS)
		return (fret);
	return (COMMON_SUCCESS);
}

int	process_special_char_check_pars(t_parser_data *d)
{
	if (d->prompt[d->pi] == '(')
	{
		if (handle_open_par(d, *d->pi) == COMMON_FAILURE)
		{
			print_shell_error(NULL, PARSER_ERR_MSG);
			return (COMMON_FAILURE);
		}
		else
			return (CONTINUE);
	}
	else if (d->prompt[d->pi] == ')')
	{
		if (handle_close_par(d) == COMMON_FAILURE)
		{
			print_shell_error(NULL, PARSER_ERR_MSG);
			return (COMMON_FAILURE);
		}
		return (BREAK);
	}
	return (COMMON_SUCCESS);
}
