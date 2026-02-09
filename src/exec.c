#include "exec.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"
#include "token.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int	exec_ops(t_shell *msh, int *ret_code)
{
	(void)ret_code;

	int				fret;
	size_t			ti; // Token index

	t_parser_data	*pd;
	
	ti = 0;
	pd = msh->pd;
	fret = COMMON_SUCCESS;
	// Go through all token-operands
	while (ti < pd->token_cnt)
	{
		t_token		*token;
		t_operand	*op;

		token = &pd->tokens[ti];
		if (token->type == OPERAND)
		{
			int			fret;
			size_t		opt_i; // Operand's token index
			t_op_token	*op_tok;

			op = token->op;

			fret = exec_divide_op_str_on_tokens(op);
			if (fret != COMMON_SUCCESS)
				return (fret);

			fret = exec_update_op_tokens_quote_intervals(op);
			if (fret != COMMON_SUCCESS)
				return (fret);

			opt_i = 0;
			while (opt_i < op->token_cnt)
			{
				op_tok = &op->tokens[opt_i];

				// >, >>, <, <<
				if (exec_token_is_redirect(op_tok))
				{
					++opt_i; // Just skip this operand token
					continue ;
				}
				else if (exec_token_is_assignment(op, op_tok)) // Assignment
				{
					fret = exec_process_assignment(msh, op, op_tok, &opt_i);
					if (fret == EXEC_CONTINUE)
						continue ;
					else if (fret != COMMON_SUCCESS)
						return (fret);
				}

				// Argument/Redirection's operand
				// Current operand's token is a regular argument or a redirection opernad
				// ...

				++opt_i;
			} // while (opt_i < op->token_cnt)


		} // if (token->type == OPERAND)
	
		++ti;

	} // while (ti < pd->token_cnt)

	return (fret);
}

/* Here we consider only the first '=' found.
 * For example:
 *     # VAR'='=1
 * in this case we'll get VAR' and '=1 but
 * the variable name simply will not be
 * considered as correct later! So it's fine.
 * Another example:
 *     # VAR==1
 * here we'll just get VAR and =1 */
int	exec_process_assignment(t_shell *msh, t_operand *op, t_op_token *op_tok, size_t *opt_i)
{
	int		fret;
	char	*var[2];
	bool	f_varname_correct;

	f_varname_correct = false;

	// Split operand's token string by '='
	fret = div2_str_by_delim(op_tok->cnt, '=', &var[EXEC_VAR_NAME], &var[EXEC_VAR_VALUE]);
	if (fret != COMMON_SUCCESS)
		return (fret);

	// Check LHS (variable's name) for correctness
	if (is_variable_name_correct(var[EXEC_VAR_NAME]))
	{
		// We're gonna treat this token as an assignment
		f_varname_correct = true;
		fret = exec_expand_varname(msh, op, op_tok, var);
		if (fret == EXEC_CONTINUE)
		{
			++(*opt_i);
			return (EXEC_CONTINUE);
		}
		if (fret != COMMON_SUCCESS) // We consider EXEC_CONTINUE here
			return (fret);
	}
	if (f_varname_correct)
	{
		++(*opt_i); // Next token
		return (EXEC_CONTINUE);
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
int	exec_expand_varname(t_shell *msh, t_operand *op, t_op_token *op_tok, char *var[])
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
		env_set(op->my_env, ft_strdup(var[EXEC_VAR_NAME]),
			ft_strdup(var[EXEC_VAR_NAME]), LOCAL);
		free(var[EXEC_VAR_NAME]);
		free(var[EXEC_VAR_VALUE]);
		return (EXEC_CONTINUE); // Go to the next operand's token
	}
	// Initialize `qmask` and `exp_res` vectors
	if (!exec_vectors_init(vec_pair, ft_strlen(tok_str) - i + 1))
	{
		free(var[EXEC_VAR_NAME]);
		free(var[EXEC_VAR_VALUE]);
		return (COMMON_SYS_ERR);
	}
	// We actually can use op_tok->quotes because if we reach this point
	// it means ALL quote intervals are located after the first '=' (unquoted)
	exec_expand_varname_loop(msh, tok_str, var[EXEC_VAR_NAME], vec_pair);
	// Create a local environment variable and add it into
	// the array of variables of this operand `vars`
	env_set(op->my_env, ft_strdup(var[EXEC_VAR_NAME]),
		ft_strdup(vec_pair[EXEC_EXP_RES]->data), LOCAL);
	free(var[EXEC_VAR_NAME]);
	free(var[EXEC_VAR_VALUE]);
	return (COMMON_SUCCESS);
}

/* Loop through token symbols.
 * Again, we already excluded cases like VAR"IABLE=VALUE"
 * here the variable name will not be considered as correct
 * and we'll be treating this token as an argument */
void	exec_expand_varname_loop(t_shell *msh, char *tok_str, char *var_name, t_vector *vec_pair[])
{
	size_t		i;
	size_t		eqsign_ind;
	t_ind_type	state; // State of the current operand's token symbol

	i = 0;
	state = IND_QNONE;
	eqsign_ind = ft_abs(ft_strchr(tok_str, '=') - tok_str);
	while (i < ft_strlen(tok_str))
	{	
		if (tok_str[i] == '"') // If current symbol is a double syntax quote
			exec_process_double_quote(tok_str, &i, vec_pair, state);
		else if (tok_str[i] == '\'') // If current symbol is a single syntax quote
			exec_process_single_quote(tok_str, &i, vec_pair, state);
		else if (exec_tilde_found(tok_str, i, eqsign_ind, state)) // If ~ was found
			exec_expand_tilde(msh, vec_pair, state);
		else if (tok_str[i] == '$' && state != IND_QSINGLE) // If $ was found
			exec_expand_variable(msh, vec_pair, var_name, state);
		else // Current symbol is just a regular symbol (except quote)
		{
			vector_push_back_char(vec_pair[EXEC_EXP_RES], tok_str[i]);
			vector_push_back_char(vec_pair[EXEC_QMASK], (char)state);
		}
		++i;
	}
	vector_push_back_char(vec_pair[EXEC_EXP_RES], '\0');
}

void	exec_process_double_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type state)
{
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[0];
	qmask = vec_pair[1];
	if (state == IND_QNONE) // We enter the double quotation interval
	{
		state = IND_QDOUBLE;
	}
	else if (state == IND_QDOUBLE) // We left the double quotation interval
	{
		state = IND_QNONE;
	}
	else if (state == IND_QSINGLE) // This double quote is just a data quote
	{
		// Copy this double quote (cause it's just a regular symbol)
		vector_push_back_char(exp_res, tok_str[*i]);
		// Mark the corresponding index in qmask[] as IND_QSINGLE
		vector_push_back_char(qmask, (char)IND_QSINGLE);
	}
}

void	exec_process_single_quote(char *tok_str, size_t *i, t_vector *vec_pair[], t_ind_type state)
{
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[0];
	qmask = vec_pair[1];
	if (state == IND_QNONE) // We enter the single quotation interval
	{
		state = IND_QSINGLE;
	}
	else if (state == IND_QSINGLE) // We left the single quotation interval
	{
		state = IND_QNONE;
	}
	else if (state == IND_QDOUBLE) // This single quote is just a data quote
	{
		// Copy this single quote (cause it's just a regular symbol)
		vector_push_back_char(exp_res, tok_str[*i]);
		// Mark the corresponding index in qmask[] as IND_QDOUBLE
		vector_push_back_char(qmask, (char)IND_QDOUBLE);
	}
}
