#include "prompt_parser.h"

/* The first token is always NONE because
 * we always check previous token value */
int	parser_init(t_parser_data *d, char *rline_buf)
{
	d->token_cnt = 1;
	d->prompt = rline_buf;
	remove_right_spaces(d->prompt);
	if (ops_init(d->ops) == -1) // Initialize operators array
		return (COMMON_SYS_ERR);
	open_par_init(d);
	close_par_init(d);
	tokens_init(d);
	pars_init(d->pars);
	return (COMMON_SUCCESS);
}

void	parser_free(t_parser_data *d)
{
	ops_free(d->ops);
}
