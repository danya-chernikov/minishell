#include "quote.h"
#include "prompt_parser.h"

#include <stdio.h>
#include <stdlib.h>

/* Quotes parser for prompt.
 * ADD OVERFLOW CHECK! */
bool quotes_parser(char *str, t_quote_int *quotes, size_t *qpair_cnt)
{
	int		fret;
	bool	f_dquote;	// Double quote found flag
	bool	f_squote;	// Single quote found flag
	size_t	slen;
	size_t	i;

	i = 0;
	f_dquote = false;
	f_squote = false;
	*qpair_cnt = 0;
	fret = COMMON_SUCCESS;
	slen = ft_strlen(str);
	while (i < slen)
	{
		if (str[i] == '"') // If we encounter with a double quote
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
					quotes[*qpair_cnt].li = i;
					quotes[*qpair_cnt].type = DOUBLE_QUOTE;
					f_dquote = true;
				}
			}
			else // We have already found earlier a qouble quote
			{
				// Let's close this interval and add it into the quote intervals array
				quotes[*qpair_cnt].ri = i;
				++(*qpair_cnt);
				f_dquote = false;
				// Go further by prompt
			}
		}
		else if (str[i] == '\'') // If we encounter with a single quote
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
					quotes[*qpair_cnt].li = i;
					quotes[*qpair_cnt].type = SINGLE_QUOTE;
					f_squote = true;
				}
			}
			else // We have already found earlier a qouble quote
			{
				// Let's close this interval and add it into the quote intervals array
				quotes[*qpair_cnt].ri = i;
				++(*qpair_cnt);
				f_squote = false;
				// Go further by prompt
			}
		}
		++i;
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
 * quote interval (single or double quotes) */
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

bool	is_inside_op_quotes_single(t_operand *op, size_t op_i)
{
	size_t	qi;
	
	qi = 0;
	while (qi < op->qpair_cnt)
	{
		if (op->quotes[qi].type == SINGLE_QUOTE)
		{
			if (op_i > op->quotes[qi].li && op_i < op->quotes[qi].ri)
				return (true);
		}
		++qi;
	}
	return (false);
}

bool	is_inside_op_quotes_double(t_operand *op, size_t op_i)
{
	size_t	qi;
	
	qi = 0;
	while (qi < op->qpair_cnt)
	{
		if (op->quotes[qi].type == DOUBLE_QUOTE)
		{
			if (op_i > op->quotes[qi].li && op_i < op->quotes[qi].ri)
				return (true);
		}
		++qi;
	}
	return (false);
}

/* If mark[i] == 1 we'll not include this symbol */
int	remove_syntax_quotes(char *str,  t_quote_int *quotes, size_t qpair_cnt)
{
	size_t	slen;
	size_t	i;
	size_t	w;
	size_t	qi;
	char	*mark;

	if (!str || !quotes || qpair_cnt == 0)
		return (COMMON_SUCCESS);

	slen = ft_strlen(str);
	if (slen == 0)
		return (COMMON_SUCCESS);
	
	mark = (char *)ft_calloc(slen, sizeof(char));
	if (!mark)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}

	qi = 0;
	while (qi < qpair_cnt)
	{
		if (quotes[qi].li < slen)
			mark[quotes[qi].li] = 1;
		if (quotes[qi].ri < slen)
			mark[quotes[qi].ri] = 1;
		++qi;
	}

	w = 0;
	i = 0;
	while (i < slen)
	{
		if (!mark[i])
			str[w++] = str[i];
		++i;
	}
	str[w] = '\0';
	
	free(mark);
	return (COMMON_SUCCESS);
}
