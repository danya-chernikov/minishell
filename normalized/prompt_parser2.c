/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_parser2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:44:28 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 16:40:04 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt_parser.h"
#include "expansion.h"

/* Handles opening-parenthesis */
int	handle_open_par(t_parser_data *d, int *opar_ind)
{
	size_t	last_opar_ind;
	size_t	prompt_len;
	size_t	i;
	int		fret;

	if (d->tokens[d->token_cnt - 1].type != PIPE
		&& d->tokens[d->token_cnt - 1].type != OPEN_PAR
		&& d->tokens[d->token_cnt - 1].type != AND
		&& d->tokens[d->token_cnt - 1].type != OR
		&& d->tokens[d->token_cnt - 1].type != NONE)
		return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
	d->tokens[d->token_cnt].type = OPEN_PAR;
	d->tokens[d->token_cnt].start_pi = *opar_ind;
	++d->token_cnt;
	prompt_len = ft_strlen(d->prompt);
	d->open_par[d->opar_cnt] = *opar_ind;
	d->pars[d->par_cnt].first = *opar_ind;
	++d->par_cnt;
	d->pi = *opar_ind + 1;
	++d->opar_cnt;
	fret = handle_open_par_check_errors(d, &last_opar_ind, prompt_len);
	if (fret != COMMON_SUCCESS)
		return (fret);
	--d->opar_cnt;
	return (COMMON_SUCCESS);
}

int	handle_open_par_check_errors(t_parser_data *d, size_t *last_opar_ind,
		size_t prompt_len)
{
	int	i;

	if (d->pi == prompt_len)
		return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
	if (parser_engine(d) != COMMON_SUCCESS)
		return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
	if (d->cpar_cnt == 0)
		return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
	*last_opar_ind = d->open_par[d->opar_cnt - 1];
	i = 0;
	while (i < (size_t)d->cpar_cnt)
	{
		if (d->close_par[i][0] > *last_opar_ind
			&& d->close_par[i][1] == NOT_CLOSED_PAR)
			break ;
		++i;
	}
	if (i == (size_t)d->cpar_cnt)
		return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
	d->pi = d->close_par[i][0] + 1;
	d->close_par[i][1] = CLOSED_PAR;
	return (COMMON_SUCCESS);
}

/* last_cpar_ind - last closing-parenthesis index */
int	handle_close_par(t_parser_data *d)
{
	size_t	last_cpar_ind;
	size_t	pair_opar_ind;
	size_t	i;

	if (d->tokens[d->token_cnt - 1].type != OPERAND
		&& d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
		return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
	if (d->opar_cnt == 0)
		return (print_shell_error(NULL, PARSER_ERR_MSG), COMMON_FAILURE);
	d->tokens[d->token_cnt].type = CLOSE_PAR;
	d->tokens[d->token_cnt].start_pi = d->pi;
	++d->token_cnt;
	last_cpar_ind = d->pi;
	i = 0;
	pair_opar_ind = i;
	while (i < d->opar_num)
	{
		if (d->all_open_pars[i][0] < last_cpar_ind
			&& d->all_open_pars[i][1] == NOT_CLOSED_PAR)
			pair_opar_ind = i;
		++i;
	}
	d->pars[pair_opar_ind].second = d->pi;
	d->all_open_pars[pair_opar_ind][1] = CLOSED_PAR;
	return (COMMON_SUCCESS);
}
