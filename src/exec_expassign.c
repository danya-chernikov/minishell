#include "exec.h"
#include "shell.h"
#include "token.h"
#include "operand.h"
#include "expansion.h"
#include "aux_common.h"
#include "prompt_parser.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static int	check_if_heredoc(t_shell *msh, t_operand *op,
				t_op_token *op_tok, size_t *opt_i);

/* Performs expansions and assignments for all second-level
 * tokens of this operand token. First, it divides the
 * operand token token into second-level tokens by spaces,
 * then updates the quote intervals of each second-level
 * token. Depending on the second-level token type, it
 * performs variable and wildcard expansions and variable
 * assignments (if the entire second-level token is a
 * variable assignment) on the content of each second-level.
 *     opt_i - operand's token index */
int	do_all_expansions_assignments(t_shell *msh, t_token *token)
{
	int			fret;
	int			redir_cnt;
	size_t		opt_i;
	t_operand	*op;

	op = token->op;
	fret = exp_divide_op_str_on_tokens(op);
	if (fret != COMMON_SUCCESS)
		return (fret);
	fret = exp_update_op_tokens_quote_intervals(op);
	if (fret != COMMON_SUCCESS)
		return (fret);
	opt_i = 0;
	redir_cnt = 0;
	while (opt_i < op->token_cnt)
	{
		fret = do_expassign_2nd_lvl_token(msh, op, &opt_i, &redir_cnt);
		if (fret == CONTINUE)
			continue ;
		else if (fret != COMMON_SUCCESS)
			return (fret);
		++opt_i;
	}
	fret = COMMON_SUCCESS;
	return (fret);
}

int	do_expassign_2nd_lvl_token(t_shell *msh, t_operand *op,
		size_t *opt_i, int *redir_cnt)
{
	int			fret;
	t_op_token	*op_tok;

	fret = COMMON_SUCCESS;
	op_tok = &op->tokens[*opt_i];
	if (exp_token_is_redirect(op_tok))
	{
		op->tokens[*opt_i + 1].redir_ind = *redir_cnt;
		op->tokens[*opt_i].redir_ind = -1;
		++(*redir_cnt);
		++(*opt_i);
		return (CONTINUE);
	}
	else if (exp_token_is_assignment(op, op_tok))
	{
		op->tokens[*opt_i].redir_ind = -1;
		fret = exp_process_assignment(msh, op, op_tok, opt_i);
		if (fret == CONTINUE)
			return (CONTINUE);
		else if (fret != COMMON_SUCCESS)
			return (fret);
	}
	fret = check_if_heredoc(msh, op, op_tok, opt_i);
	return (fret);
}

static int	check_if_heredoc(t_shell *msh, t_operand *op,
				t_op_token *op_tok, size_t *opt_i)
{
	int	fret;

	op->tokens[*opt_i].redir_ind = -1;
	if (*opt_i > 0 && exp_token_is_heredoc(&op->tokens[*opt_i - 1]))
	{
		++(*opt_i);
		return (CONTINUE);
	}
	fret = exp_process_argredir(msh, op, op_tok, opt_i);
	if (fret != COMMON_SUCCESS)
		return (fret);
	return (COMMON_SUCCESS);
}
