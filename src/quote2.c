/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:29:36 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 03:34:12 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "operand.h"

#include <stdbool.h>

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
