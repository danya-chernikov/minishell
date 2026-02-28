/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:43:58 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 05:38:09 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt_parser.h"
#include "aux_common.h"

/* The first token is always NONE because
 * we always check previous token value */
int	parser_init(t_parser_data *d, char *rline_buf)
{
	int	fret;

	d->pi = 0;
	d->op_cnt = 0;
	d->qpair_cnt = 0;
	d->par_cnt = 0;
	d->opar_cnt = 0;
	d->cpar_cnt = 0;
	d->token_cnt = 1;
	d->prompt = rline_buf;
	remove_right_spaces(d->prompt);
	fret = ops_init(d->ops);
	if (fret != COMMON_SUCCESS)
		return (fret);
	tokens_init(d);
	pars_init(d->pars);
	return (fret);
}

void	parser_free(t_parser_data *d)
{
	ops_free(d->ops);
}

/* Parses the user's prompt string by connecting all
 * operands with pipes and launching or exiting subshells
 * when encountering '(' or ')' parentheses, respectively.
 *     opar_ind - prompt index of the open-parenthesis
 *				  that goes after pipe */
int	parser_engine(t_parser_data *d)
{
	size_t	prompt_len;
	int		fret;
	int		opar_ind;

	fret = COMMON_SUCCESS;
	if (!d->prompt)
		return (fret);
	prompt_len = ft_strlen(d->prompt);
	if (prompt_len == 0)
		return (fret);
	while (d->pi < prompt_len)
	{
		fret = parser_engine_loop_body(d, &opar_ind, prompt_len);
		if (fret == BREAK)
			return (COMMON_SUCCESS);
		if (fret == CONTINUE)
			continue ;
		if (fret != COMMON_SUCCESS)
			return (fret);
		++d->pi;
	}
	return (COMMON_SUCCESS);
}

int	parser_engine_loop_body(t_parser_data *d, int *opar_ind, size_t prompt_len)
{
	if (d->prompt[d->pi] == ' ')
		skip_spaces(d->prompt, &d->pi);
	if (d->pi == prompt_len)
		return (BREAK);
	if (!is_spec_char(d->prompt[d->pi]) || is_inside_quotes(d, d->pi))
		return (process_nonspecial_char(d, opar_ind, prompt_len));
	return (process_special_char(d, prompt_len));
}
