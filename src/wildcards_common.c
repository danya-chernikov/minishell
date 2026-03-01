/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_common.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:19:55 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 12:39:58 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"
#include "operand.h"
#include "aux_common.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>
#include <stdbool.h>

static int	wc_collapse_conseq_asterisks_loop_body(t_vector *exp_res,
				t_vector *qmask, size_t *rw_inds, bool *f_prev_ast);

/* Collapse consequtive unquoted asterisks in `exp_res` and update `qmask`.
 * We traverse the original vector-string `exp_res` from left to right
 * using index `i` (read_ind) and at the same time, we write the result
 * to the beginning of the same vector using index `write_ind`. In other
 * words, `read_ind` reads all characters of the original `exp_res`, and
 * `write_ind` writes only those we wanna keep. In the end, everything
 * that needs to be removed (extra *) is simply not written, and everything
 * we keep gets compacted toward the begining. It runs in O(n) btw.
 *     f_prev_ast - if true means prevoius symbol was asterisk out of quotes.
 *     rw_inds[WC_READ_IND=0]	- read_ind
 *     rw_inds[WC_WRITE_IND=1]	- write_ind */
void	wc_collapse_conseq_asterisks(t_vector *exp_res, t_vector *qmask)
{
	size_t	expres_len;
	size_t	rw_inds[2];
	bool	f_prev_ast;

	rw_inds[WC_READ_IND] = 0;
	rw_inds[WC_WRITE_IND] = 0;
	expres_len = vector_strlen(exp_res);
	f_prev_ast = false;
	while (rw_inds[WC_READ_IND] < expres_len)
	{
		if (wc_collapse_conseq_asterisks_loop_body(exp_res,
				qmask, rw_inds, &f_prev_ast) == CONTINUE)
			continue ;
		++rw_inds[WC_WRITE_IND];
		++rw_inds[WC_READ_IND];
	}
	wc_trim_vectors(exp_res, qmask, rw_inds[WC_WRITE_IND]);
}

static int	wc_collapse_conseq_asterisks_loop_body(t_vector *exp_res,
		t_vector *qmask, size_t *rw_inds, bool *f_prev_ast)
{
	char	expres_char;
	char	qmask_char;

	expres_char = *((char *)vector_at(exp_res, rw_inds[WC_READ_IND]));
	qmask_char = *((char *)vector_at(qmask, rw_inds[WC_READ_IND]));
	if (wc_is_asterisk(expres_char, qmask_char) && *f_prev_ast)
	{
		++rw_inds[WC_READ_IND];
		return (CONTINUE);
	}
	if (rw_inds[WC_WRITE_IND] != rw_inds[WC_READ_IND])
	{
		vector_si(exp_res, rw_inds[WC_WRITE_IND], &expres_char);
		vector_si(qmask, rw_inds[WC_WRITE_IND], &qmask_char);
	}
	*f_prev_ast = wc_is_asterisk(expres_char, qmask_char);
	return (COMMON_SUCCESS);
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
