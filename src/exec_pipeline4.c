/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:14:55 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:14:56 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "shell.h"
#include "expansion.h"
#include "prompt_parser.h"

#include "error.h"

static void	save_operand(t_pipeline *pl, int *st_i, size_t *j);
static void	save_open_par(t_pipeline *pl, int *st_i, size_t *j, int *close_i);

/* The condition `close_i == CLOSE_PAR_NOT_FOUND` practically should
 * never happen, but just in case let's check it.. */
int	pl_fill_stages(t_parser_data *pd, size_t l, size_t r, t_pipeline *pl)
{
	size_t	j;
	int		st_i;
	int		close_i;

	j = l;
	st_i = 0;
	while (j <= r && st_i < pl->stages_num)
	{
		if (pd->tokens[j].type == OPERAND)
			save_operand(pl, &st_i, &j);
		else if (pd->tokens[j].type == OPEN_PAR)
		{
			close_i = pl_find_close(pd, j, r);
			if (close_i == CLOSE_PAR_NOT_FOUND)
				return (COMMON_FAILURE);
			save_open_par(pl, &st_i, &j, &close_i);
		}
		else
			++j;
	}
	if (st_i != pl->stages_num)
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}

static void	save_operand(t_pipeline *pl, int *st_i, size_t *j)
{
	pl->stages[*st_i].l = *j;
	pl->stages[*st_i].r = *j;
	++(*st_i);
	++(*j);
}

static void	save_open_par(t_pipeline *pl, int *st_i, size_t *j, int *close_i)
{
	pl->stages[*st_i].l = *j;
	pl->stages[*st_i].r = (size_t)(*close_i);
	++(*st_i);
	*j = (size_t)(*close_i) + 1;
}
