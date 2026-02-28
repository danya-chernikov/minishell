#include "exec.h"
#include "prompt_parser.h"

/* The logic of skipping the entire term when a
 * boolean logic condition kicked in. It finds the
 * term border (doing it the same way as `find_term_stop()`
 * does) and returns the index where to jump next:
 *     1. on && or || operators which terminates the term skipped;
 *     2. on the next token after this index;
 *     3. on stops on ')' (if the skipping inside the parentheses
 *		  ended with a closing one) */
int	skip_term(t_parser_data *pd, size_t s, size_t r)
{
	size_t	j;
	int		d;

	j = s;
	d = 0;
	while (j <= r)
	{
		if (pd->tokens[j].type == OPEN_PAR)
			++d;
		else if (pd->tokens[j].type == CLOSE_PAR)
		{
			if (d == 0)
				return (j);
			--d;
		}
		else if (d == 0 && (pd->tokens[j].type == AND
				|| pd->tokens[j].type == OR))
			return (j);
		++j;
	}
	return (r + 1);
}

int	check_depth_limit(t_parser_data *pd, size_t l, size_t r, int base)
{
	size_t	j;
	int		d;

	j = l;
	d = 0;
	while (j <= r)
	{
		if (pd->tokens[j].type == OPEN_PAR)
		{
			++d;
			if (base + d > MAX_DEPTH)
				return (COMMON_FAILURE);
		}
		else if (pd->tokens[j].type == CLOSE_PAR)
			--d;
		++j;
	}
	return (COMMON_SUCCESS);
}
