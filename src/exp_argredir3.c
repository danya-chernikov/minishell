/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_argredir3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:09:59 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:10:00 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"

#include "vector.h"

void	exp_argredir_handle_dollar(t_shell *msh,
		t_vector *vec_pair[], t_exp_ar_loop *ctx)
{
	char	dlr_varname[MAX_ENV_VAL_LEN];

	if (!exp_extract_dlr_varname(dlr_varname, ctx->tok_str, &ctx->i))
	{
		exp_push_arg_char(vec_pair, '$', ctx->state);
		return ;
	}
	exp_expand_variable(msh, vec_pair, dlr_varname, ctx->state);
}
