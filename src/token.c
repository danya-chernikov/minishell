#include "token.h"
#include "prompt_parser.h"

void	tokens_init(t_parser_data *d)
{
	size_t	i;

	i = 0;
	while (i < MAX_TOKENS_NUM)
	{
		d->tokens[i].op = NULL;
		++i; 
	}
	d->tokens[0].type = NONE;
	d->tokens[0].start_pi = 0;
}

/* Accepts the index of a parenthesis in `d->pars` and
 * returns the index of this parenthesis in the array
 * of tokens `d->tokens`. If there is no parenthesis
 * with such an index in `d->tokens`, returns -1
 *
 *     ti			- token index
 *     prompt_ind	- index of token in prompt */
t_ll	get_token_by_prompt_ind(t_parser_data *d, size_t prompt_ind)
{
	t_ll	ti;

	ti = 0;
	while ((size_t)ti < d->token_cnt)
	{
		if (prompt_ind == d->tokens[ti].start_pi)
			return (ti);
		++ti;
	}
	return (-1);
}
