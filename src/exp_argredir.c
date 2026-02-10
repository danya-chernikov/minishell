#include "expansion.h"
#include "shell.h"
#include "operand.h"
#include "wildcards.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

int		exp_process_argredir(t_shell *msh, t_op_token *op_tok)
{
	char		*tok_str;
	t_vector	*vec_pair[2];
	
	tok_str = op_tok->cnt;

	if (!exp_vectors_init(vec_pair, ft_strlen(tok_str) + 1))
		return (COMMON_SYS_ERR);

	exp_expand_argredir_loop(msh, tok_str, vec_pair);

	// Expand wildcards
	{	
		size_t		i;
		bool		f_wc_expand;
		t_vector	*exp_res;
		t_vector	*qmask;
		t_vector	wc_mask;

		i = 0;
		f_wc_expand = true;
		vector_init(&wc_mask, CHAR, 64);
		exp_res = vec_pair[EXP_RES];
		qmask = vec_pair[QMASK];
		while (i < vector_strlen(exp_res))
		{
			// Means the entire token is a globbing mask)
			if (*((char *)vector_at(exp_res, i)) == '*' &&
				*((t_ind_type *)vector_at(qmask, i)) == IND_QNONE)
			{
				if (i > 0 && *((char *)vector_at(exp_res, i - 1)) == '*')
				{
					++i;
					continue ;
				}
				vector_push_back_char(wc_mask, '*');
			}
			else
			++i;
		}
	}

	exp_vectors_free(vec_pair);
	return (COMMON_SUCCESS);
}

void	exp_expand_argredir_loop(t_shell *msh, char *tok_str, t_vector *vec_pair[])
{
	size_t		i;
	size_t		slen;
	char		dlr_varname[MAX_ENV_VAL_LEN];
	t_ind_type	state;

	i = 0;
	slen = ft_strlen(tok_str);
	state = IND_QNONE;
	while (i < slen)
	{	
		if (tok_str[i] == '"')
			exp_process_double_quote(tok_str, &i, vec_pair, state);
		else if (tok_str[i] == '\'')
			exp_process_single_quote(tok_str, &i, vec_pair, state);
		else if (exp_tilde_found_argredir(tok_str, i, state))
			exp_expand_tilde(msh, vec_pair, state);
		else if (tok_str[i] == '$' && state != IND_QSINGLE)
		{
			exp_extract_dlr_varname(dlr_varname, tok_str, &i);
			exp_expand_variable(msh, vec_pair, dlr_varname, state);
		}
		else
		{
			vector_push_back_char(vec_pair[EXP_RES], tok_str[i]);
			vector_push_back_char(vec_pair[QMASK], (char)state);
		}
		++i;
	}
	vector_push_back_char(vec_pair[EXP_RES], '\0');
}
