/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_preparation2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:57:06 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 13:59:04 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* Updates quote intervals for each operand's token */
int	exp_update_op_tokens_quote_intervals(t_operand *op)
{
	int			fret;
	size_t		ti;
	t_op_token	*op_tok;

	ti = 0;
	fret = COMMON_SUCCESS;
	while (ti < op->token_cnt)
	{
		op_tok = &op->tokens[ti];
		fret = quotes_parser(op_tok->cnt, op_tok->quotes, &op_tok->qpair_cnt);
		if (fret != COMMON_SUCCESS)
			return (fret);
		++ti;
	}
	return (fret);
}
