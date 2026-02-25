#include "redirect_parser.h"
#include "operand.h"

#include <stdlib.h>

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* Extracts and cleans the heredoc delimiter.
 * Sets expansion flag based on presence of quotes.*/
int	get_hd_delimiter(t_operand *op, size_t *wi)
{
	int			fret;
	char		*delim;
	size_t		qpair_cnt;
	t_quote_int	*quotes;

	delim = malloc(MAX_HD_DELIM_LEN);
	if (!delim)
		return (perror("malloc"), COMMON_SYS_ERR);
	op->redirs[op->red_cnt].hd.delim = delim;
	if (extract_redir_path(op, wi, delim, MAX_HD_DELIM_LEN) == -1)
		return (COMMON_FAILURE);
	if (contains_quote(delim))
		op->redirs[op->red_cnt].hd.f_expand_body = false;
	else
		op->redirs[op->red_cnt].hd.f_expand_body = true;
	quotes = malloc(MAX_QUOTES_NUM * sizeof (*quotes));
	if (!quotes)
		return (perror("malloc"), COMMON_SYS_ERR);
	fret = quotes_parser(delim, quotes, &qpair_cnt);
	if (fret == COMMON_SUCCESS)
		fret = remove_syntax_quotes(delim, quotes, qpair_cnt);
	free(quotes);
	return (fret);
}
