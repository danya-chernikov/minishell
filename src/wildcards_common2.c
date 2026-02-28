/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_common2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:37:27 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 12:39:02 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

#include <stdlib.h>

void	wc_free_res(char ***wc_res)
{
	size_t	i;	

	i = 0;
	while (i < WC_MAX_FILES_NUM)
	{
		free((*wc_res)[i]);
		++i;
	}
	free(*wc_res);
}
