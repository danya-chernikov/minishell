/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:26:03 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 05:39:18 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt_parser.h"

/* Let's say the first element of the
 * pair is the opening parenthesis,
 * and the second is the closing one */
void	pars_init(t_pair *pars)
{
	size_t	i;

	i = 0;
	while (i < MAX_PAR_NUM)
	{
		pars[i].first = NONE_PAR_IND;
		pars[i].second = NONE_PAR_IND;
		++i;
	}
}

int	open_par_init(t_parser_data *d)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(d->prompt))
	{
		if (d->prompt[i] == '(' && !is_inside_quotes(d, i))
		{
			if (d->opar_num >= MAX_PAR_NUM)
			{
				print_shell_error(NULL, MAX_PAR_NUM_ERR_MSG);	
				return (COMMON_FAILURE);
			}
			d->all_open_pars[d->opar_num][0] = i;
			d->all_open_pars[d->opar_num][1] = NOT_CLOSED_PAR;
			++d->opar_num;
		}
		++i;
	}
	i = 0;
	while (i < d->opar_num)
	{
		d->open_par[i] = NONE_INDEX;
		++i;
	}
	return (COMMON_SUCCESS);
}

/* Counts all closing parentheses
 * and remembers their indexes */
int	close_par_init(t_parser_data *d)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(d->prompt))
	{
		if (d->prompt[i] == ')' && !is_inside_quotes(d, i))
		{
			if (d->cpar_cnt >= MAX_PAR_NUM)
			{
				print_shell_error(NULL, MAX_PAR_NUM_ERR_MSG);	
				return (COMMON_FAILURE);
			}
			d->close_par[d->cpar_cnt][0] = i;
			d->close_par[d->cpar_cnt][1] = NOT_CLOSED_PAR;
			++d->cpar_cnt;
		}
		++i;
	}
	return (COMMON_SUCCESS);
}

/* Accepts the index of a parenthesis in the array of tokens
 * `d->tokens` and returns the index of this parenthesis in
 * `d->pars`. If there is no parenthesis with such a token
 * index in `d->pars`, returns -1
 *
 *     pind   - index of token in prompt
 *     pars_i - index in `d->pars` */
t_ll	get_par_by_prompt_ind(t_parser_data *d, size_t pind, t_par_type ptype)
{
	t_ll	pars_ind;

	pars_ind = 0;
	while ((size_t)pars_ind < d->par_cnt)
	{
		if (ptype == OPENING_PAR)
		{
			if (pind == (size_t)d->pars[pars_ind].first)
				return (pars_ind);
		}
		else if (ptype == CLOSING_PAR)
		{
			if (pind == (size_t)d->pars[pars_ind].second)
				return (pars_ind);
		}
		++pars_ind;
	}
	return (-1);
}
