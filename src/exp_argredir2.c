/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_argredir2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:37:50 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:12:35 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "operand.h"
#include "wildcards.h"

#include "vector.h"
#include "error.h"

static void	push_back_exp_qmask(t_vector *field_pair[],
				t_vector *vec_pair[], size_t *i);

int	expand_one_field(t_operand *op, t_vector *vec_pair[],
		size_t start, size_t end)
{
	int			fret;
	size_t		i;
	char		**wc_res;
	t_vector	*field_pair[2];

	if (end <= start)
		return (COMMON_SUCCESS);
	if (exp_vectors_init(field_pair, end - start + 1) != COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	i = start;
	while (i < end)
		push_back_exp_qmask(field_pair, vec_pair, &i);
	vector_push_back_char(field_pair[EXP_RES], '\0');
	wc_collapse_conseq_asterisks(field_pair[EXP_RES], field_pair[QMASK]);
	if (wc_alloc_res(&wc_res) != COMMON_SUCCESS)
		return (exp_vectors_free(field_pair), COMMON_SYS_ERR);
	fret = expand_wildcards(wc_res,
			(char *)field_pair[EXP_RES]->data, field_pair[QMASK]);
	if (fret == COMMON_SUCCESS)
		fret = append_wc_matches(op, wc_res);
	wc_free_res(&wc_res);
	exp_vectors_free(field_pair);
	return (fret);
}

static void	push_back_exp_qmask(t_vector *field_pair[],
				t_vector *vec_pair[], size_t *i)
{
	vector_push_back_char(field_pair[EXP_RES],
		((char *)vec_pair[EXP_RES]->data)[*i]);
	vector_push_back_char(field_pair[QMASK],
		((char *)vec_pair[QMASK]->data)[*i]);
	++(*i);
}

int	append_split_fields(t_operand *op, t_vector *vec_pair[])
{
	int		fret;
	size_t	i;
	size_t	start;
	char	*exp_str;
	char	*qmask_str;

	exp_str = (char *)vec_pair[EXP_RES]->data;
	qmask_str = (char *)vec_pair[QMASK]->data;
	i = 0;
	while (exp_str[i] != '\0')
	{
		while (exp_str[i] == ' ' && qmask_str[i] == (char)IND_QNONE)
			++i;
		if (exp_str[i] == '\0')
			break ;
		start = i;
		while (exp_str[i] != '\0'
			&& !(exp_str[i] == ' ' && qmask_str[i] == (char)IND_QNONE))
			++i;
		fret = expand_one_field(op, vec_pair, start, i);
		if (fret != COMMON_SUCCESS)
			return (fret);
	}
	return (COMMON_SUCCESS);
}

void	exp_expand_argredir_loop(t_shell *msh, char *tok_str,
			t_vector *vec_pair[])
{
	t_exp_ar_loop	ctx;

	ctx.tok_str = tok_str;
	ctx.i = 0;
	ctx.state = IND_QNONE;
	while (ctx.tok_str[ctx.i] != '\0')
	{
		if (ctx.tok_str[ctx.i] == '"')
			exp_process_double_quote(ctx.tok_str, &ctx.i, vec_pair, &ctx.state);
		else if (ctx.tok_str[ctx.i] == '\'')
			exp_process_single_quote(ctx.tok_str, &ctx.i, vec_pair, &ctx.state);
		else if (exp_tilde_found_argredir(ctx.tok_str, ctx.i, ctx.state))
			exp_expand_tilde(msh, vec_pair, ctx.state);
		else if (ctx.tok_str[ctx.i] == '$' && ctx.state != IND_QSINGLE)
			exp_argredir_handle_dollar(msh, vec_pair, &ctx);
		else
			exp_push_arg_char(vec_pair, ctx.tok_str[ctx.i], ctx.state);
		++ctx.i;
	}
	vector_push_back_char(vec_pair[EXP_RES], '\0');
}

void	exp_push_arg_char(t_vector *vec_pair[],
		char c, t_ind_type state)
{
	vector_push_back_char(vec_pair[EXP_RES], c);
	vector_push_back_char(vec_pair[QMASK], (char)state);
}
