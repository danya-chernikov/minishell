#include "prompt_parser.h"
#include "env.h" // NEW

/* The first token is always NONE because
 * we always check previous token value */
int	parser_init(t_parser_data *d, char *rline_buf)
{
	int	fret;

	d->token_cnt = 1;
	d->prompt = rline_buf;
	remove_right_spaces(d->prompt);
	fret = ops_init(d->ops);
	if (fret != COMMON_SUCCESS) // Initialize operators array
		return (fret);
	open_par_init(d);
	close_par_init(d);
	tokens_init(d);
	pars_init(d->pars);
	return (fret);
}

void	parser_free(t_parser_data *d)
{
	ops_free(d->ops);
}
