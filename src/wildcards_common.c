/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_common.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:11:27 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/26 00:21:45 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"
#include "operand.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Collapse consequtive unquoted asterisks in `exp_res` and update `qmask`.
 * We traverse the original vector-string `exp_res` from left to right
 * using index `i` (read_ind) and at the same time, we write the result
 * to the beginning of the same vector using index `write_ind`. In other
 * words, `read_ind` reads all characters of the original `exp_res`, and
 * `write_ind` writes only those we wanna keep. In the end, everything
 * that needs to be removed (extra *) is simply not written, and everything
 * we keep gets compacted toward the begining. It runs in O(n) btw.
 *     f_prev_ast - if true means prevoius symbol was asterisk out of quotes */
void	wc_collapse_conseq_asterisks(t_vector *exp_res, t_vector *qmask)
{
	size_t	read_ind;
	size_t	write_ind;
	bool	f_prev_ast;
	char	expres_char;
	char	qmask_char;

	read_ind = 0;
	write_ind = 0;
	f_prev_ast = false;
	while (read_ind < vector_strlen(exp_res))
	{
		expres_char = *((char *)vector_at(exp_res, read_ind));
		qmask_char = *((char *)vector_at(qmask, read_ind));
		if (!wc_is_asterisk(expres_char, qmask_char) && f_prev_ast)
		{
			wc_update_vectors(exp_res, qmask_char, &write_ind, read_ind);
			f_prev_ast = wc_is_asterisk(expres_char, qmask_char);
		}
		++read_ind;
	}
	wc_trim_vectors(exp_res, qmask, write_ind);
}

void	wc_update_vectors(t_vector *exp_res, t_vector *qmask_char,
				size_t *w, size_t r)
{
	char	c;
	char	m;

	c = *((char *)vector_at(exp, r));
	m = *((char *)vector_at(qmk, r));
	vector_si(exp, *w, &c);
	vector_si(qmk, *w, &m);
	(*w)++;
}

bool	wc_is_asterisk(char expres_char, char qmask_char)
{
	if (expres_char == '*' && qmask_char == (char)IND_QNONE)
		return (true);
	return (false);
}

void	wc_trim_vectors(t_vector *exp_res, t_vector *qmask, size_t new_len)
{
	char	null;

	null = '\0';
	if (vector_size(exp_res) == new_len)
		vector_push_back_char(exp_res, '\0');
	else
		vector_si(exp_res, new_len, &null);
	while (vector_size(exp_res) > new_len + 1)
		vector_pop_back(exp_res);
	while (vector_size(qmask) > new_len)
		vector_pop_back(qmask);
}

int	wc_alloc_res(char ***wc_res)
{
	long long	i;

	*wc_res = (char **)ft_calloc(WC_MAX_FILES_NUM, sizeof (char *));
	if (!(*wc_res))
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	i = 0;
	while (i < WC_MAX_FILES_NUM)
	{
		(*wc_res)[i] = (char *)ft_calloc(WC_MAX_FILENAME_LEN, sizeof (char));
		if (!(*wc_res)[i])
		{
			while (i > 0)
				free((*wc_res)[--i]);
			free(*wc_res);
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++i;
	}
	return (COMMON_SUCCESS);
}
