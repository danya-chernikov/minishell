#include "expansion.h"
#include "shell.h"
#include "operand.h"

#include "vector.h"

void	exp_process_double_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type *state)
{
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[EXP_RES];
	qmask = vec_pair[QMASK];
	if (*state == IND_QNONE) // We enter the double quotation interval
	{
		*state = IND_QDOUBLE;
	}
	else if (*state == IND_QDOUBLE) // We left the double quotation interval
	{
		*state = IND_QNONE;
	}
	else if (*state == IND_QSINGLE) // This double quote is just a data quote
	{
		// Copy this double quote (cause it's just a regular symbol)
		vector_push_back_char(exp_res, tok_str[*i]);
		// Mark the corresponding index in qmask[] as IND_QSINGLE
		vector_push_back_char(qmask, (char)IND_QSINGLE);
	}
}

void	exp_process_single_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type *state)
{
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[EXP_RES];
	qmask = vec_pair[QMASK];
	if (*state == IND_QNONE) // We enter the single quotation interval
	{
		*state = IND_QSINGLE;
	}
	else if (*state == IND_QSINGLE) // We left the single quotation interval
	{
		*state = IND_QNONE;
	}
	else if (*state == IND_QDOUBLE) // This single quote is just a data quote
	{
		// Copy this single quote (cause it's just a regular symbol)
		vector_push_back_char(exp_res, tok_str[*i]);
		// Mark the corresponding index in qmask[] as IND_QDOUBLE
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
	// Expand ~ by copying its value into the new array
	while (j < ft_strlen(tilde))
	{
		vector_push_back_char(exp_res, tilde[j]); // Copying...
		// Assign the corresponding index in qmask[] the curret `state` value
		vector_push_back_char(qmask, (char)state);
		++j;
	}
}

void	exp_expand_variable(t_shell *msh, t_vector *vec_pair[], char *var_name, t_ind_type state)
{
	size_t		j;
	char		*var;
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[EXP_RES];
	qmask = vec_pair[QMASK];
	var = env_get_val(&msh->env, var_name);
	if (!var)
		var = ft_strdup("");
	j = 0;
	// Expand $ by copying the value of `var` into the new array
	while (j < ft_strlen(var))
	{
		vector_push_back_char(exp_res, var[j]); // Copying...
		// Assign the corresponding index in qmask[] the curret `state` value
		vector_push_back_char(qmask, (char)state);
		++j;
	}
}

/* I think overflow here is impossible, because when we create an environment
 * variable we check for its maximum length (See set_rest_env_vars() function)*/
void	exp_extract_dlr_varname(char *dlr_varname, char *tok_str, size_t *i)
{
	size_t	j;

	j = 0;
	++(*i);
	// Exceptions
	if (*i < ft_strlen(tok_str) &&
		(tok_str[*i] == '$' || tok_str[*i] == '?' ||
		tok_str[*i] == '#' || tok_str[*i] == '*'))
	{
		dlr_varname[j] = tok_str[*i];
		++j;
	}
	else
	{
		while (*i < ft_strlen(tok_str) && is_varname_symbol_permitted(tok_str[*i]))
		{
			dlr_varname[j] = tok_str[*i];
			++(*i);
			++j;
		}
		--(*i); // Because later `i` will be incremented in caller's loop
	}
	dlr_varname[j] = '\0';
}
