#include "expansion.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"
#include "token.h"

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
int	exp_process_assignment(t_shell *msh, t_operand *op, t_op_token *op_tok, size_t *opt_i)
{
	int		fret;
	char	*var_name;
	char	*var_value;
	bool	f_varname_correct;

	f_varname_correct = false;

	// Split operand's token string by '='
	fret = div2_str_by_delim(op_tok->cnt, '=', &var_name, &var_value);
	if (fret != COMMON_SUCCESS)
		return (fret);
	free(var_value); // We'll never use it

	// Check LHS (variable's name) for correctness
	if (is_variable_name_correct(var_name))
	{
		// We're gonna treat this token as an assignment
		f_varname_correct = true;
		fret = exp_expand_varname(msh, op, op_tok, var_name);
		if (fret == CONTINUE)
		{
			++(*opt_i);
			return (CONTINUE);
		}
		if (fret != COMMON_SUCCESS) // We consider CONTINUE here
			return (fret);
	}
	if (f_varname_correct)
	{
		++(*opt_i); // Next token
		return (CONTINUE);
	}
	// If variable's name is not correct
	// We're gonna treat this token as an argument	
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
int	exp_expand_varname(t_shell *msh, t_operand *op, t_op_token *op_tok, char *var_name)
{
	size_t		i;				// Symbol index in operand's token
	size_t		eqsign_ind;
	char		*tok_str;		// Pointer to operand's token string
	t_vector	*vec_pair[2];	// To pass our lovely friend Norminette
		
	tok_str = op_tok->cnt;
	eqsign_ind = ft_abs(ft_strchr(tok_str, '=') - tok_str);
	i = eqsign_ind + 1;
	// If after '=' goes nothing ( things like "VAR=" )
	if (i == ft_strlen(tok_str))
	{
		// env_set() already has overflow checks
		if (env_set(op->my_env, var_name, ft_strdup(""), LOCAL) != COMMON_SUCCESS)
			return (COMMON_FAILURE);
		return (CONTINUE); // Go to the next operand's token
	}
	// Initialize `qmask` and `exp_res` vectors
	if (!exp_vectors_init(vec_pair, ft_strlen(tok_str) - i + 1))
		return (COMMON_SYS_ERR);
	// We actually can use op_tok->quotes because if we reach this point
	// it means ALL quote intervals are located after the first '=' (unquoted)
	exp_expand_varname_loop(msh, tok_str, vec_pair);
	// Create a local environment variable and add it into
	// the array of variables of this operand `vars`
	if (env_set(op->my_env, var_name,
		ft_strdup(vec_pair[EXP_RES]->data), LOCAL) != COMMON_SUCCESS)
	{
		exp_vectors_free(vec_pair);
		return (COMMON_FAILURE);
	}
	exp_vectors_free(vec_pair);
	return (COMMON_SUCCESS);
}

/* Loop through token symbols.
 * Again, we already excluded cases like VAR"IABLE=VALUE"
 * here the variable name will not be considered as correct
 * and we'll be treating this token as an argument.
 *     i			- Operand's token index;
 *     state		- State of the current operand's token symbol;
 *     dlr_varname	- Dollar variable name */
void	exp_expand_varname_loop(t_shell *msh, char *tok_str, t_vector *vec_pair[])
{
	size_t		i;
	size_t		eqsign_ind;
	char		dlr_varname[MAX_ENV_VAL_LEN];
	t_ind_type	state;

	i = 0;
	state = IND_QNONE;
	eqsign_ind = ft_abs(ft_strchr(tok_str, '=') - tok_str);
	while (i < ft_strlen(tok_str))
	{	
		if (tok_str[i] == '"')
			exp_process_double_quote(tok_str, &i, vec_pair, state);
		else if (tok_str[i] == '\'')
			exp_process_single_quote(tok_str, &i, vec_pair, state);
		else if (exp_tilde_found_assign(tok_str, i, eqsign_ind, state))
			exp_expand_tilde(msh, vec_pair, state);
		else if (tok_str[i] == '$' && state != IND_QSINGLE)
		{
			// We need to extract the variable name first
			// Just go to the right of the $ and copy all the characters
			// until we encounter an invalid one (that is not permitted)
			exp_extract_dlr_varname(dlr_varname, tok_str, &i);
			exp_expand_variable(msh, vec_pair, dlr_varname, state);
		}
		else // Current symbol is just a regular symbol (except quote)
		{
			vector_push_back_char(vec_pair[EXP_RES], tok_str[i]);
			vector_push_back_char(vec_pair[QMASK], (char)state);
		}
		++i;
	}
	vector_push_back_char(vec_pair[EXP_RES], '\0');
}
