
#include "expansion.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"
#include "token.h"
#include "aux_common.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

void	exp_push_assign_char(t_vector *vec_pair[],
		char c, t_ind_type state)
{
	vector_push_back_char(vec_pair[EXP_RES], c);
	vector_push_back_char(vec_pair[QMASK], (char)state);
}

void	exp_assign_handle_dollar(t_shell *msh,
		t_vector *vec_pair[], t_exp_as_loop *ctx)
{
	char	dlr_varname[MAX_ENV_VAL_LEN];

	if (!exp_extract_dlr_varname(dlr_varname, ctx->tok_str, &ctx->i))
	{
		exp_push_assign_char(vec_pair, '$', ctx->state);
		return ;
	}
	exp_expand_variable(msh, vec_pair, dlr_varname, ctx->state);
}
