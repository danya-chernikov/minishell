/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_common2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:49:03 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:49:03 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "operand.h"

#include "vector.h"

void	exp_expand_variable(t_shell *msh, t_vector *vec_pair[],
			char *var_name, t_ind_type state)
{
	size_t		j;
	char		*var;
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[EXP_RES];
	qmask = vec_pair[QMASK];
	if (!var_name || var_name[0] == '\0')
	{
		vector_push_back_char(exp_res, '$');
		vector_push_back_char(qmask, (char)state);
		return ;
	}
	var = env_get_val(&msh->env, var_name);
	if (!var)
		var = "";
	j = 0;
	while (j < ft_strlen(var))
	{
		vector_push_back_char(exp_res, var[j]);
		vector_push_back_char(qmask, (char)state);
		++j;
	}
}
