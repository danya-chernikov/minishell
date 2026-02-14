#include "wildcards.h"
#include "operand.h"
#include "aux_common.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>
#include <stdbool.h>

/* Fills `res_files` with files matching the mask.
 *     resf_cnt	- counter for the result files array;
 *     file		- pointer to the current file;
 *     fi		- file index */
int	wc_sift_files_by_mask(t_wc_data *wcd, char **res_files, char *files[], size_t file_cnt)
{
	size_t		resf_cnt;
	size_t		fi;
	int			fret;

	fret = COMMON_SUCCESS;
	resf_cnt = 0;
	fi = 0;
	while (fi < file_cnt)
	{
		wcd->file = files[fi];
		wcd->file_len = ft_strlen(files[fi]);
		fret = wc_check_file(wcd, res_files, &resf_cnt);
		if (fret != COMMON_SUCCESS)
			return (fret);
		++fi;
	}
	fret = wc_check_zero_matches(wcd, res_files, &resf_cnt);
	if (fret != COMMON_SUCCESS)
		return (fret);
	res_files[resf_cnt] = NULL;
	return (fret);
}

/* Checks the current file for a match with the mask.
 *     f_cancel	- if true we'll not add the current file in the result;
 *     mi		- mask index */
int	wc_check_file(t_wc_data	*wcd, char **res_files, size_t *resf_cnt)
{
	bool	f_cancel;
	
	wcd->mi = 0;
	f_cancel = false;
	wc_check_file_loop(wcd, &f_cancel);
	if (wcd->mi < wcd->mask_len)
		f_cancel = true;
	if (!f_cancel)
	{
		res_files[*resf_cnt] = ft_strdup(wcd->file);
		if (!res_files[*resf_cnt])
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++(*resf_cnt);
	}
	return (COMMON_SUCCESS);
}

void	wc_check_file_loop(t_wc_data *wcd, bool *f_cancel)
{
	wcd->ci = 0;
	while (wcd->ci < wcd->file_len)
	{
		if (wcd->mi == wcd->mask_len)
		{
			if (wcd->ci < wcd->file_len)
				*f_cancel = true;
			break ;
		}
		if (wcd->mask[wcd->mi] == '*' &&
			*((t_ind_type *)vector_at(wcd->qmask, wcd->mi)) == IND_QNONE)
		{
			if (wc_match_asterisk(wcd) == BREAK)
				break ;
		}
		else
		{
			if (wc_match_regular_symbol(wcd, f_cancel) == BREAK)
				break ;
		}
		++wcd->ci;
	}
}

int	wc_match_asterisk(t_wc_data *wcd)
{
	if (wcd->mi + 1 == wcd->mask_len)
	{
		++wcd->mi;
		return (BREAK);
	}
	while (wcd->ci < wcd->file_len)
	{
		if (wcd->file[wcd->ci] == wcd->mask[wcd->mi + 1])
		{
			++wcd->mi;
			return (BREAK);
		}
		++wcd->ci;
	}
	--wcd->ci;
	return (COMMON_SUCCESS);
}

int	wc_match_regular_symbol(t_wc_data *wcd, bool *f_cancel)
{
	if (wcd->file[wcd->ci] != wcd->mask[wcd->mi])
	{
		*f_cancel = true;
		return (BREAK);
	}
	else
		++wcd->mi;
	return (COMMON_SUCCESS);
}
