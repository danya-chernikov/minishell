#include "expansion.h"

#include "vector.h"

void	exp_handle_assign_dollar(t_exp_assign_ctx *ctx)
{
	char	dlr_varname[MAX_ENV_VAL_LEN];

	if (!exp_extract_dlr_varname(dlr_varname,
			ctx->tok_str, ctx->i))
		push_assign_mask(ctx->vec_pair, '$', *ctx->state);
	else
		exp_expand_variable(ctx->msh,
			ctx->vec_pair, dlr_varname, *ctx->state);
}

/* Helper to push character and mask to vectors */
void	push_assign_mask(t_vector *vec_pair[], char c, t_ind_type state)
{
	vector_push_back_char(vec_pair[EXP_RES], c);
	vector_push_back_char(vec_pair[QMASK], (char)state);
}
