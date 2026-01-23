#include "quote.h"
# include "prompt_parser.h"

bool quotes_parser(t_parser_data *d)
{
	bool	f_noerr;
	bool	f_dquote;	// Double quote found flag
	bool	f_squote;	// Single quote found flag
	size_t	prompt_len;
	size_t	pi;			// Prompt index

	pi = 0;
	f_noerr = true;
	f_dquote = false;
	f_squote = false;
	prompt_len = ft_strlen(d->prompt);
	while (pi < prompt_len)
	{
		if (d->prompt[pi] == '"') // If we encounter with a double quote
		{
			// If it's the first double quote found
			if (f_dquote == false)
			{
				if (f_squote == true) // But this double quote is inside the single qoute
				{
					// Just ingnore this double quote
				}
				else
				{
					d->quotes[d->qpair_cnt].li = pi;
					d->quotes[d->qpair_cnt].type = DOUBLE_QUOTE;
					f_dquote = true;
				}
			}
			else // We have already found earlier a qouble quote
			{
				// Let's close this interval and add it into the quote intervals array
				d->quotes[d->qpair_cnt].ri = pi;
				++d->qpair_cnt;
				f_dquote = false;
				// Go further by prompt
			}
		}
		else if (d->prompt[pi] == '\'') // If we encounter with a single quote
		{
			// If it's the first single quote found
			if (f_squote == false)
			{
				if (f_dquote == true) // But this single quote is inside the double qoute
				{
					// Just ingnore this single quote
				}
				else
				{
					d->quotes[d->qpair_cnt].li = pi;
					d->quotes[d->qpair_cnt].type = SINGLE_QUOTE;
					f_squote = true;
				}
			}
			else // We have already found earlier a qouble quote
			{
				// Let's close this interval and add it into the quote intervals array
				d->quotes[d->qpair_cnt].ri = pi;
				++d->qpair_cnt;
				f_squote = false;
				// Go further by prompt
			}
		}
		++pi;
	} // while (pi < prompt_len)
	
	// If after parsing there are any unmatched
	// quotes (quotes without pairs) left 
	if (f_squote || f_dquote)
		handle_prompt_parser_ext_error(&f_noerr, UNMATCH_QUOTES_ERR_MSG);
	return (f_noerr);
}
