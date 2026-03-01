/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 11:53:07 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 11:56:01 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

#include "error.h"

void	quotes_parser_init(bool *qflags, size_t *qpair_cnt, int *fret)
{
	qflags[QSINGLE] = false;
	qflags[QDOUBLE] = false;
	*qpair_cnt = 0;
	*fret = COMMON_SUCCESS;
}
