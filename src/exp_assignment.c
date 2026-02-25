/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_assignment.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 18:39:56 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 23:19:32 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"
#include "token.h"
#include "aux_common.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

/* Here we consider only the first '=' found.
 * For example:
 *     # VAR'='=1
 * in this case we'll get VAR' and '=1 but
 * the variable name simply will not be
 * considered as correct later! So it's fine.
 * Another example:
 *     # VAR==1
 * here we'll just get VAR and =1 */
int	exp_process_assignment(t_shell *msh, t_operand *op,
	t_op_token *op_tok, size_t *opt_i)
{
	int		fret;
	char	*var_name;
	char	*var_value;

	fret = div2_str_by_delim(op_tok->cnt, '=', &var_name, &var_value);
	if (fret != COMMON_SUCCESS)
		return (fret);
	free(var_value);
	if (is_variable_name_correct(var_name))
	{
		fret = exp_expand_varname(msh, op, op_tok, var_name);
		if (fret == CONTINUE || fret == COMMON_SUCCESS)
		{
			++(*opt_i);
			return (CONTINUE);
		}
		return (fret);
	}
	return (COMMON_SUCCESS);
}

/* vec_pair[0] - exp_res - Quote mask;
 * vec_pair[1] - qmask - Array that stores expansion results.
 *
 * We do not have to expand anything in variable's name.
 * So let's start with the next symbol after '='.
 * Always search for the first '=' occurence.
 *
 * if (i == ft_strlen(tok_str))
 *     We don't have to expand anything.
 *     We just create a variable with empty value and we
 *     don't care about an error cause it can't be critical */
int	exp_expand_varname(t_shell *msh, t_operand *op, t_op_token *op_tok,
			char *var_name)
{
	size_t		i;
	size_t		eqsign_ind;
	t_vector	*vec_pair[2];

	eqsign_ind = ft_abs(ft_strchr(op_tok->cnt, '=') - op_tok->cnt);
	i = eqsign_ind + 1;
	if (i == ft_strlen(tok_str))
	{
		if (env_set(op->my_env, var_name, ft_strdup(""), LOCAL)
			!= COMMON_SUCCESS)
			return (COMMON_FAILURE);
		return (CONTINUE);
	}
	if (!exp_vectors_init(vec_pair, ft_strlen(tok_str) - i + 1))
		return (COMMON_SYS_ERR);
	exp_expand_varname_loop(msh, op_tok->cnt, vec_pair);
	if (set_expanded_var(op, var_name, v) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}

/* Handles the assignment of expanded values to a variable name.
 * Only sets the variable if the f_per_cmd flag is false.*/
int	set_expanded_var(t_operand *op, char *name, t_vector *vec_pair[])
{
	int	res;

	res = COMMON_SUCCESS;
	if (!op->f_per_cmd)
	{
		res = env_set(op->my_env, name,
				ft_strdup(vec_pair[EXP_RES]->data), LOCAL);
	}
	exp_vectors_free(vec_pair);
	return (res);
}

/* Loop through token symbols.
 * Again, we already excluded cases like VAR"IABLE=VALUE"
 * here the variable name will not be considered as correct
 * and we'll be treating this token as an argument.
 *     i			- Operand's token index;
 *     state		- State of the current operand's token symbol;
 *     dlr_varname	- Dollar variable name */
void	exp_expand_varname_loop(t_shell *msh, char *tok_str,
		t_vector *vec_pair[])
{
	size_t		i;
	size_t		eqsing_ind;
	t_ind_type	state;

	state = IND_QNONE;
	eqsing_ind = ft_abs(ft_strchr(tok_str, '=') - tok_str);
	i = eqsing_ind + 1;
	while (tok_str[i])
	{
		exp_handle_assign_char(msh, tok_str,
			vec_pair, &i, &state, eqsing_ind);
		i++;
	}
	vector_push_back_char(vec_pair[EXP_RES], '\0');
}

void	exp_handle_assign_char(t_exp_assign_ctx *ctx)
{
	char	c;

	c = ctx->tok_str[*ctx->i];
	if (c == '"')
		exp_process_double_quote(ctx->tok_str,
			ctx->i, ctx->vec_pair, ctx->state);
	else if (c == '\'')
		exp_process_single_quote(ctx->tok_str,
			ctx->i, ctx->vec_pair, ctx->state);
	else if (exp_tilde_found_assign(ctx->tok_str,
			*ctx->i, ctx->eqsing_ind, *ctx->state))
		exp_expand_tilde(ctx->msh,
			ctx->vec_pair, *ctx->state);
	else if (c == '$' && *ctx->state != IND_QSINGLE)
		exp_handle_assign_dollar(ctx);
	else
		push_assign_mask(ctx->vec_pair, c, *ctx->state);
}
