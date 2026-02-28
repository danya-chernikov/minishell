/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:19:37 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 12:19:41 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcards.h"

#include "vector.h"
#include "error.h"

/* Expands wildcards using mask.
 *     res_files	- the result of globbing;
 *     files		- initial files in current directory */
int	expand_wildcards(char **res_files, char *mask, t_vector *qmask)
{
	char		*files[WC_MAX_FILES_NUM];
	size_t		file_cnt;
	int			fret;
	t_wc_data	wcd;

	wcd.mask = mask;
	wcd.mask_len = ft_strlen(mask);
	wcd.qmask = qmask;
	fret = wc_get_curdir_files(files, &file_cnt);
	if (fret != COMMON_SUCCESS)
		return (fret);
	wc_sift_files_by_mask(&wcd, res_files, files, file_cnt);
	wc_free_curdir_files(files, file_cnt);
	return (COMMON_SUCCESS);
}
