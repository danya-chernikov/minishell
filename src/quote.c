#include "quote.h"
#include "prompt_parser.h"

/* Quotes parser for prompt */
bool quotes_parser(t_parser_data *d)
{
	int		fret;
	bool	f_dquote;	// Double quote found flag
	bool	f_squote;	// Single quote found flag
	size_t	prompt_len;
	size_t	pi;			// Prompt index

	pi = 0;
	f_dquote = false;
	f_squote = false;
	fret = COMMON_SUCCESS;
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
	{
		print_shell_error(NULL, UNMATCH_QUOTES_ERR_MSG);
		fret = COMMON_FAILURE;
	}
	return (fret);
}

/* Quotes parser for operand's `name` (its content) */
bool operand_quotes_parser(t_operand *op)
{
	int		fret;
	bool	f_dquote;	// Double quote found flag
	bool	f_squote;	// Single quote found flag
	size_t	op_len;
	size_t	op_i;		// Prompt index

	op_i = 0;
	f_dquote = false;
	f_squote = false;
	fret = COMMON_SUCCESS;
	op_len = ft_strlen(op->name);
	while (op_i < op_len)
	{
		if (op->name[op_i] == '"') // If we encounter with a double quote
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
					op->quotes[op->qpair_cnt].li = op_i;
					op->quotes[op->qpair_cnt].type = DOUBLE_QUOTE;
					f_dquote = true;
				}
			}
			else // We have already found earlier a qouble quote
			{
				// Let's close this interval and add it into the quote intervals array
				op->quotes[op->qpair_cnt].ri = op_i;
				++op->qpair_cnt;
				f_dquote = false;
				// Go further by prompt
			}
		}
		else if (op->name[op_i] == '\'') // If we encounter with a single quote
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
					op->quotes[op->qpair_cnt].li = op_i;
					op->quotes[op->qpair_cnt].type = SINGLE_QUOTE;
					f_squote = true;
				}
			}
			else // We have already found earlier a qouble quote
			{
				// Let's close this interval and add it into the quote intervals array
				op->quotes[op->qpair_cnt].ri = op_i;
				++op->qpair_cnt;
				f_squote = false;
				// Go further by prompt
			}
		}
		++op_i;
	} // while (pi < prompt_len)
	
	// If after parsing there are any unmatched
	// quotes (quotes without pairs) left 
	if (f_squote || f_dquote)
	{
		print_shell_error(NULL, UNMATCH_QUOTES_ERR_MSG);
		fret = COMMON_FAILURE;
	}
	return (fret);
}

/* Check if index `op_i` belongs to any operand's `name`
 * quote interval */
bool	is_inside_op_quotes(t_operand *op, size_t op_i)
{
	size_t	qi;
	
	qi = 0;
	while (qi < op->qpair_cnt)
	{
		if (op_i > op->quotes[qi].li && op_i < op->quotes[qi].ri)
			return (true);
		++qi;
	}
	return (false);
}
