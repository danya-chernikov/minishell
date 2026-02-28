/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:26:13 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 05:44:35 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "prompt_parser.h"

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>

static void	remove_syntax_quotes_loop(t_quote_int *quotes, char *mark,
				size_t qpair_cnt, size_t slen);
static int	process_double_quote(t_quote_int *quotes, size_t *qpair_cnt,
				bool *qflags, size_t i);
static int	process_single_quote(t_quote_int *quotes, size_t *qpair_cnt,
				bool *qflags, size_t i);

/* Quotes parser for prompt.
 * ADD OVERFLOW CHECK!
 *     f_dquote=qflags[QDOUBLE] - Double quote found flag;
 *     f_squote=qflags[QSINGLE] - Single quote found flag */
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
			fret = process_double_quote(quotes, qpair_cnt, qflags, i);
		else if (str[i] == '\'')
			fret = process_single_quote(quotes, qpair_cnt, qflags, i);
		if (fret == COMMON_FAILURE)
			return (COMMON_FAILURE);
		++i;
	}
	if (qflags[QSINGLE] || qflags[QDOUBLE])
	{
		print_shell_error(NULL, UNMATCH_QUOTES_ERR_MSG);
		fret = COMMON_FAILURE;
	}
	return (fret);
}

static int	process_double_quote(t_quote_int *quotes, size_t *qpair_cnt,
				bool *qflags, size_t i)
{
	if (qflags[QDOUBLE] == false)
	{
		if (!qflags[QSINGLE])
		{
			if (*qpair_cnt >= MAX_QUOTES_NUM)
			{
				print_shell_error(NULL, MAX_QUOTES_NUM_ERR_MSG);
				return (COMMON_FAILURE);
			}
			quotes[*qpair_cnt].li = i;
			quotes[*qpair_cnt].type = DOUBLE_QUOTE;
			qflags[QDOUBLE] = true;
		}
	}
	else
	{
		if (*qpair_cnt >= MAX_QUOTES_NUM)
		{
			print_shell_error(NULL, MAX_QUOTES_NUM_ERR_MSG);
			return (COMMON_FAILURE);
		}
		quotes[*qpair_cnt].ri = i;
		++(*qpair_cnt);
		qflags[QDOUBLE] = false;
	}
	return (COMMON_SUCCESS);
}

static int	process_single_quote(t_quote_int *quotes, size_t *qpair_cnt,
				bool *qflags, size_t i)
{
	if (qflags[QSINGLE] == false)
	{
		if (!qflags[QDOUBLE])
		{
			if (*qpair_cnt >= MAX_QUOTES_NUM)
			{
				print_shell_error(NULL, MAX_QUOTES_NUM_ERR_MSG);
				return (COMMON_FAILURE);
			}
			quotes[*qpair_cnt].li = i;
			quotes[*qpair_cnt].type = SINGLE_QUOTE;
			qflags[QSINGLE] = true;
		}
	}
	else
	{
		if (*qpair_cnt >= MAX_QUOTES_NUM)
		{
			print_shell_error(NULL, MAX_QUOTES_NUM_ERR_MSG);
			return (COMMON_FAILURE);
		}
		quotes[*qpair_cnt].ri = i;
		++(*qpair_cnt);
		qflags[QSINGLE] = false;
	}
	return (COMMON_SUCCESS);
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
