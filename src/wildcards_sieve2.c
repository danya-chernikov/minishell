/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_sieve2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:42:52 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 12:42:53 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>
#include <stdbool.h>

int	wc_check_zero_matches(t_wc_data *wcd, char **res_files, size_t *resf_cnt)
{
	if (*resf_cnt == 0)
	{
		ft_strlcpy(res_files[*resf_cnt], wcd->mask, WC_MAX_FILENAME_LEN);
		++(*resf_cnt);
	}
	return (COMMON_SUCCESS);
}
