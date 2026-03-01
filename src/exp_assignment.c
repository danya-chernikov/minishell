/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_assignment.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:39:44 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:39:45 by dchernik         ###   ########.fr       */
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
	if (!is_variable_name_correct(var_name))
		return (free(var_name), COMMON_SUCCESS);
	fret = exp_expand_varname(msh, op, op_tok, var_name);
	if (fret != COMMON_SUCCESS && fret != CONTINUE)
		return (fret);
	++(*opt_i);
	return (CONTINUE);
}

int	exp_expand_varname(t_shell *msh, t_operand *op,
		t_op_token *op_tok, char *var_name)
{
	size_t		i;
	int			fret;
	char		*tok_str;
	t_vector	*vec_pair[2];

	tok_str = op_tok->cnt;
	i = exp_assign_eqind(tok_str) + 1;
	if (tok_str[i] == '\0')
	{
		if (env_set(op->my_env, var_name, ft_strdup(""), LOCAL)
			!= COMMON_SUCCESS)
			return (COMMON_FAILURE);
		return (CONTINUE);
	}
	if (exp_vectors_init(vec_pair, ft_strlen(tok_str) - i + 1)
		!= COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	exp_expand_varname_loop(msh, tok_str, vec_pair);
	fret = exp_store_var(op, var_name, vec_pair);
	exp_vectors_free(vec_pair);
	return (fret);
}

void	exp_expand_varname_loop(t_shell *msh, char *tok_str,
		t_vector *vec_pair[])
{
	t_exp_as_loop	ctx;

	ctx.tok_str = tok_str;
	ctx.i = exp_assign_eqind(tok_str) + 1;
	ctx.state = IND_QNONE;
	while (ctx.tok_str[ctx.i] != '\0')
	{
		if (ctx.tok_str[ctx.i] == '"')
			exp_process_double_quote(ctx.tok_str, &ctx.i, vec_pair, &ctx.state);
		else if (ctx.tok_str[ctx.i] == '\'')
			exp_process_single_quote(ctx.tok_str, &ctx.i, vec_pair, &ctx.state);
		else if (exp_tilde_found_assign(ctx.tok_str, ctx.i,
				ctx.i - 1, ctx.state))
			exp_expand_tilde(msh, vec_pair, ctx.state);
		else if (ctx.tok_str[ctx.i] == '$' && ctx.state != IND_QSINGLE)
			exp_assign_handle_dollar(msh, vec_pair, &ctx);
		else
			exp_push_assign_char(vec_pair, ctx.tok_str[ctx.i], ctx.state);
		++ctx.i;
	}
	vector_push_back_char(vec_pair[EXP_RES], '\0');
}

size_t	exp_assign_eqind(char *tok_str)
{
	return ((size_t)ft_abs(ft_strchr(tok_str, '=') - tok_str));
}

int	exp_store_var(t_operand *op, char *var_name,
		t_vector *vec_pair[])
{
	if (op->f_per_cmd)
		return (free(var_name), COMMON_SUCCESS);
	if (env_set(op->my_env, var_name,
			ft_strdup((char *)vec_pair[EXP_RES]->data), LOCAL)
		!= COMMON_SUCCESS)
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}
