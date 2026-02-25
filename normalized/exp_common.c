/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_common.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 10:15:08 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/26 00:20:37 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "operand.h"

#include "vector.h"

/* Processes double quotes state and updates the qmask vector.*/
void	exp_process_double_quote(char *tok_str, size_t *i,
									t_vector *vec_pair[], t_ind_type *state)
{
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[EXP_RES];
	qmask = vec_pair[QMASK];
	if (*state == IND_QNONE)
	{
		*state = IND_QDOUBLE;
	}
	else if (*state == IND_QDOUBLE)
	{
		*state = IND_QNONE;
	}
	else if (*state == IND_QSINGLE)
	{
		vector_push_back_char(exp_res, tok_str[*i]);
		vector_push_back_char(qmask, (char)IND_QSINGLE);
	}
}

/* Processes single quotes state and updates the qmask vector.*/
void	exp_process_single_quote(char *tok_str, size_t *i,
									t_vector *vec_pair[], t_ind_type *state)
{
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[EXP_RES];
	qmask = vec_pair[QMASK];
	if (*state == IND_QNONE)
	{
		*state = IND_QSINGLE;
	}
	else if (*state == IND_QSINGLE)
	{
		*state = IND_QNONE;
	}
	else if (*state == IND_QDOUBLE)
	{
		vector_push_back_char(exp_res, tok_str[*i]);
		vector_push_back_char(qmask, (char)IND_QDOUBLE);
	}
}

/* Expands the tilde symbol to the HOME environment variable value.*/
void	exp_expand_tilde(t_shell *msh, t_vector *vec_pair[], t_ind_type state)
{
	size_t		j;
	char		*tilde;
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[EXP_RES];
	qmask = vec_pair[QMASK];
	tilde = env_get_val(&msh->env, "~");
	j = 0;
	while (j < ft_strlen(tilde))
	{
		vector_push_back_char(exp_res, tilde[j]);
		vector_push_back_char(qmask, (char)state);
		++j;
	}
}

/* Fetches the variable value from environment 
 * and pushes it to expansion result.*/
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

/* Extracts the variable name after a '$' symbol.
 * Handles special variables and alphanumeric names.*/
bool	exp_extract_dlr_varname(char *dlr_varname, const char *tok_str,
									size_t *i)
{
	size_t	j;
	size_t	p;

	j = 0;
	dlr_varname[0] = '\0';
	if (*i + 1 >= ft_strlen(tok_str))
		return (false);
	if (extract_special_var(dlr_varname, tok_str[*i + 1], i, *i))
		return (true);
	p = *i + 1;
	while (p < ft_strlen(tok_str) && is_varname_symbol_permitted(tok_str[p]))
	{
		if (j + 1 >= MAX_ENV_VAL_LEN)
			break ;
		dlr_varname[j++] = tok_str[p++];
	}
	dlr_varname[j] = '\0';
	if (j == 0)
		return (false);
	*i = p - 1;
	return (true);
}
