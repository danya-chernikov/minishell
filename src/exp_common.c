/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_common.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:42:01 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:52:51 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "operand.h"

#include "vector.h"

static int	check_special_vars(const char *tok_str, char *dlr_varname,
				size_t *i, size_t k);

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

bool	exp_extract_dlr_varname(char *dlr_varname, const char *tok_str,
			size_t *i)
{
	size_t	len;
	size_t	j;
	size_t	k;
	size_t	p;

	j = 0;
	k = *i;
	len = ft_strlen(tok_str);
	dlr_varname[0] = '\0';
	if (k + 1 >= len)
		return (false);
	if (check_special_vars(tok_str, dlr_varname, i, k))
		return (true);
	p = k + 1;
	while (p < len && is_varname_symbol_permitted(tok_str[p]))
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

static int	check_special_vars(const char *tok_str, char *dlr_varname,
				size_t *i, size_t k)
{
	char	c;

	c = tok_str[k + 1];
	if (c == '$' || c == '?' || c == '#' || c == '*')
	{
		dlr_varname[0] = c;
		dlr_varname[1] = '\0';
		*i = k + 1;
		return (true);
	}
	return (false);
}
