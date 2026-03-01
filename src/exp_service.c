/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_service.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:35:17 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:36:43 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "operand.h"

#include "vector.h"
#include "libft.h"
#include "error.h"

/* First we initialize `exp_res` and then `qmask` */
int	exp_vectors_init(t_vector *vec_pair[], size_t cap)
{
	vec_pair[EXP_RES] = NULL;
	vec_pair[QMASK] = NULL;
	vec_pair[EXP_RES] = malloc(1 * sizeof (t_vector));
	vec_pair[QMASK] = malloc(1 * sizeof (t_vector));
	if (!vec_pair[EXP_RES] || !vec_pair[QMASK])
	{
		perror("malloc");
		exp_vectors_free(vec_pair);
		return (COMMON_SYS_ERR);
	}
	if (!vector_init(vec_pair[EXP_RES], CHAR, cap))
	{
		perror("malloc");
		exp_vectors_free(vec_pair);
		return (COMMON_SYS_ERR);
	}
	if (!vector_init(vec_pair[QMASK], CHAR, cap))
	{
		perror("malloc");
		exp_vectors_free(vec_pair);
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

void	exp_vectors_free(t_vector *vec_pair[])
{
	if (vec_pair[EXP_RES])
	{
		vector_free(vec_pair[EXP_RES]);
		free(vec_pair[EXP_RES]);
	}
	if (vec_pair[QMASK])
	{
		vector_free(vec_pair[QMASK]);
		free(vec_pair[QMASK]);
	}
}

/* If state == IND_QNONE for index `i` then it's obvious
 * that tok_str[i + 1] will be unquoted or be a quote */
bool	exp_tilde_found_assign(char *tstr, size_t i,
			size_t eqsign_ind, t_ind_type state)
{
	if (tstr[i] == '~'
		&& i - 1 == eqsign_ind
		&& state == IND_QNONE
		&& (tstr[i + 1] == '/' || tstr[i + 1] == '\0'))
	{
		return (true);
	}
	return (false);
}

bool	exp_tilde_found_argredir(char *tstr, size_t i, t_ind_type state)
{
	if (state == IND_QNONE && i == 0 && tstr[i] == '~'
		&& (tstr[i + 1] == '/' || tstr[i + 1] == '\0'))
	{
		return (true);
	}
	return (false);
}

bool	exp_token_is_assignment(t_operand *op, t_op_token *op_tok)
{
	size_t	i;
	size_t	slen;

	i = 0;
	slen = ft_strlen(op_tok->cnt);
	while (i < slen)
	{
		if (op_tok->cnt[i] == '='
			&& !is_inside_quotes_uni(op_tok->quotes, op_tok->qpair_cnt, i))
		{
			if (i > 0)
				if (op->argc == 0)
					return (true);
		}
		++i;
	}
	return (false);
}
