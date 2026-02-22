#include "exec.h"
#include "shell.h"
#include "redirect.h"
#include "prompt_parser.h"

#include "error.h"

#include <unistd.h>
#include <stdlib.h>

/* Goes from left to right over the
 * tokens array and executes all token
 * operands, taking boolean logic operators
 * into account and launching subshells
 * when necessary.
 * In practice we'll never have
 * `msh->pd->token_cnt == 0` though */
int	exec_ops(t_shell *msh, int *ret_code)
{
	int	status;

	if (msh->pd->token_cnt <= 0)
	{
		msh_update_retcode(msh, 0);
		if (ret_code)
			*ret_code = 0;
		return (COMMON_SUCCESS);
	}
	status = exec_ops_range(msh, 1, msh->pd->token_cnt - 1, 0);
	msh_update_retcode(msh, status);
	if (ret_code)
		*ret_code = status;
	return (COMMON_SUCCESS);
}

int	exec_ops_range(t_shell *msh, size_t l, size_t r, int depth)
{
	t_parser_data	*pd;

	pd = msh->pd;
	if (l <= r && pd->tokens[l].type == NONE)
		++l;
	if (l > r)
		return (0);
	if (check_depth_limit(pd, l, r, depth) != COMMON_SUCCESS)
		return (SYNTAX_ERR);
	return (exec_range_loop(msh, l, r, depth));
}

/* Finds the border of the current TERM, executes this TERM as
 * a pipeline, processes the 'delimiter' (stop token) and chooses
 * the token with `i` index which the next TERM starts */
int	exec_range_loop(t_shell *msh, size_t i, size_t r, int depth)
{
	t_parser_data	*pd;
	int				stop;
	size_t			term_end;
	int				last;

	pd = msh->pd;
	last = 0;
	while (i <= r)
	{
		stop = find_term_stop(pd, i, r);
		term_end = r;
		if (stop >= 0)
			term_end = (size_t)stop - 1;
		last = pl_exec_pipeline(msh, i, term_end, depth);
		if (stop < 0 || (size_t)stop > r)
			return (last);
		if (pd->tokens[stop].type == CLOSE_PAR)
			return (last);
		i = next_i_after_logic(pd, last, stop, r);
	}
	return (last);
}

/* Returns the index of a stop-token (&&, || or ')' on
 * current depth), which terminates the current execution
 * TERM. When returns -1, means within the range we're
 * currently viewing, no stop-token has been found.
 * Answers the question: where the current TERM does terminate?
 *     d - inner depth counter for parentheses. If d == 0
 *		   we're at current parentheses level, otherwise
 *		   we're inside a parentheses	block */
int	find_term_stop(t_parser_data *pd, size_t i, size_t r)
{
	size_t	j;
	int		d;

	j = i;
	d = 0;
	while (j <= r)
	{
		if (pd->tokens[j].type == OPEN_PAR)
			++d;
		else if (pd->tokens[j].type == CLOSE_PAR)
		{
			if (d == 0)
				return ((int)j);
			--d;
		}
		else if (d == 0 && (pd->tokens[j].type == AND ||
				pd->tokens[j].type == OR))
			return ((int)j);
		++j;
	}
	return (-1);
}

/* Determines if after executing the current TERM and finding
 * && or || do we have to executing the next TERM or skip it */
size_t	next_i_after_logic(t_parser_data *pd, int last, size_t stop, size_t r)
{
	t_token_type	tok_type;
	size_t			next;

	tok_type = pd->tokens[stop].type;
	next = stop + 1;
	if (tok_type == AND && last != 0)
		return (skip_term(pd, next, r) + 1);
	if (tok_type == OR && last == 0)
		return (skip_term(pd, next, r) + 1);
	return (next);
}
