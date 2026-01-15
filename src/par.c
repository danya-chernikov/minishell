#include "prompt_parser.h"

void	init_open_par(t_parser_data *d)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(d->prompt))
	{
		if (d->prompt[i] == '(')
		{
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
}

/* Counts all closing parentheses and remembers their indexes */
void	init_close_par(t_parser_data *d)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(d->prompt))
	{
		if (d->prompt[i] == ')')
		{
			d->close_par[d->cpar_cnt][0] = i;
			d->close_par[d->cpar_cnt][1] = NOT_CLOSED_PAR;
			++d->cpar_cnt;
		}
		++i;
	}
}

/* Let's say the first element of the
 * pair is the opening parenthesis,
 * and the second is the closing one */
void	init_pars(t_pair *pars)
{
	size_t	i;

	i = 0;
	while (i < MAX_PAR_NUM)
	{
		pars->first = NONE_PAR_IND;
		pars->second = NONE_PAR_IND;
		++i;
	}
}

/* Accepts the index of a parenthesis in the array of tokens
 * `d->tokens` and returns the index of this parenthesis in
 * `d->pars`. If there is no parenthesis with such a token
 * index in `d->pars`, returns -1
 *
 *     prompt_ind	- index of token in prompt
 *     pars_i		- index in `d->pars`
 * */
t_ll	get_par_by_prompt_ind(t_parser_data *d, size_t prompt_ind, t_par_type ptype)
{
	t_ll	pars_ind;

	pars_ind = 0;
	while ((size_t)pars_ind < d->par_cnt)
	{
		// Go through opening-parentheses `d->pars[i].first`
		if (ptype == OPENING_PAR)
		{
			if (prompt_ind == (size_t)d->pars[pars_ind].first)
				return (pars_ind);
		}
		else if (ptype == CLOSING_PAR)
		{
			// Go through closing-parentheses `d->pars[i].second`
			if (prompt_ind == (size_t)d->pars[pars_ind].second)
				return (pars_ind);
		}
		++pars_ind;
	}
	return (-1);
}
