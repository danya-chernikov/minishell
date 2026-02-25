#include "quote.h"
#include "prompt_parser.h"

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>

static void	remove_syntax_quotes_loop(t_quote_int *quotes, char *mark,
				size_t qpair_cnt, size_t slen);
static void	process_double_quote(t_quote_int *quotes, size_t *qpair_cnt,
				bool *qflags, size_t i);
static void	process_single_quote(t_quote_int *quotes, size_t *qpair_cnt,
				bool *qflags, size_t i);

/* Quotes parser for prompt.
 * ADD OVERFLOW CHECK!
 *     f_dquote=qflags[QSINGLE] - Double quote found flag;
 *     f_squote=qflags[QDOUBLE] - Single quote found flag */
bool	quotes_parser(char *str, t_quote_int *quotes, size_t *qpair_cnt)
{
	int		fret;
	bool	qflags[2];
	size_t	slen;
	size_t	i;

	i = 0;
	qflags[QSINGLE] = false;
	qflags[QDOUBLE] = false;
	*qpair_cnt = 0;
	fret = COMMON_SUCCESS;
	slen = ft_strlen(str);
	while (i < slen)
	{
		if (str[i] == '"')
			process_double_quote(quotes, qpair_cnt, qflags, i);
		else if (str[i] == '\'')
			process_single_quote(quotes, qpair_cnt, qflags, i);
		++i;
	}
	if (qflags[QSINGLE] || qflags[QDOUBLE])
	{
		print_shell_error(NULL, UNMATCH_QUOTES_ERR_MSG);
		fret = COMMON_FAILURE;
	}
	return (fret);
}

static void	process_double_quote(t_quote_int *quotes, size_t *qpair_cnt,
				bool *qflags, size_t i)
{
	if (qflags[QDOUBLE] == false)
	{
		if (!qflags[QSINGLE])
		{
			quotes[*qpair_cnt].li = i;
			quotes[*qpair_cnt].type = DOUBLE_QUOTE;
			qflags[QDOUBLE] = true;
		}
	}
	else
	{
		quotes[*qpair_cnt].ri = i;
		++(*qpair_cnt);
		qflags[QDOUBLE] = false;
	}
}

static void	process_single_quote(t_quote_int *quotes, size_t *qpair_cnt,
				bool *qflags, size_t i)
{
	if (qflags[QSINGLE] == false)
	{
		if (!qflags[QDOUBLE])
		{
			quotes[*qpair_cnt].li = i;
			quotes[*qpair_cnt].type = SINGLE_QUOTE;
			qflags[QSINGLE] = true;
		}
	}
	else
	{
		quotes[*qpair_cnt].ri = i;
		++(*qpair_cnt);
		qflags[QSINGLE] = false;
	}
}

bool	is_inside_quotes_uni(t_quote_int *quotes, size_t qpair_cnt, size_t ind)
{
	size_t	qi;

	qi = 0;
	while (qi < qpair_cnt)
	{
		if (ind > quotes[qi].li && ind < quotes[qi].ri)
			return (true);
		++qi;
	}
	return (false);
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

bool	is_syntax_quote(t_quote_int *quotes, size_t qpair_cnt, size_t ind)
{
	size_t	qi;

	qi = 0;
	while (qi < qpair_cnt)
	{
		if (ind == quotes[qi].li || ind == quotes[qi].ri)
			return (true);
		++qi;
	}
	return (false);
}

/* If mark[i] == 1 we'll not include this symbol */
int	remove_syntax_quotes(char *str, t_quote_int *quotes, size_t qpair_cnt)
{
	size_t	slen;
	size_t	i;
	size_t	w;
	char	*mark;

	if (!str || !quotes || qpair_cnt == 0)
		return (COMMON_SUCCESS);
	slen = ft_strlen(str);
	if (slen == 0)
		return (COMMON_SUCCESS);
	mark = (char *)ft_calloc(slen, sizeof(char));
	if (!mark)
		return (perror("malloc"), COMMON_SYS_ERR);
	remove_syntax_quotes_loop(quotes, mark, qpair_cnt, slen);
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

static void	remove_syntax_quotes_loop(t_quote_int *quotes, char *mark,
				size_t qpair_cnt, size_t slen)
{
	size_t	qi;

	qi = 0;
	while (qi < qpair_cnt)
	{
		if (quotes[qi].li < slen)
			mark[quotes[qi].li] = 1;
		if (quotes[qi].ri < slen)
			mark[quotes[qi].ri] = 1;
		++qi;
	}
}
