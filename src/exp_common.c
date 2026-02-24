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

bool	exp_extract_dlr_varname(char *dlr_varname, const char *tok_str, size_t *i)
{
    size_t	len;
    size_t	j;
    size_t	k;
	char	c;
	size_t	p;

	j = 0;
	k = *i;
	len = ft_strlen(tok_str);
    dlr_varname[0] = '\0';

    if (k + 1 >= len)
        return (false);

    c = tok_str[k + 1];
    if (c == '$' || c == '?' || c == '#' || c == '*')
    {
        dlr_varname[0] = c;
        dlr_varname[1] = '\0';
        *i = k + 1;
        return (true);
    }

    p = k + 1;
    while (p < len && is_varname_symbol_permitted(tok_str[p]))
    {
        if (j + 1 >= MAX_ENV_VAL_LEN)
            break;
        dlr_varname[j++] = tok_str[p++];
    }
    dlr_varname[j] = '\0';

    if (j == 0)
        return (false);

    *i = p - 1;
    return (true);
}
