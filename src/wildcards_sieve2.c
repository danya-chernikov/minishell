/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_sieve2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:09:25 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 11:09:57 by jhvalenc         ###   ########.fr       */
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
