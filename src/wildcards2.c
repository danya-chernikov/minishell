#include "wildcards.h"

/* Fills `res_files` with files matching the mask.
 *     f_cancel	- if true we'll not add the current file in the result;
 *     file		- pointer to the current file;
 *     mi		- mask index */
void	wc_sift_files_by_mask(char **res_files, char *files[], size_t file_cnt, char *mask)
{
	size_t	resf_cnt;
	char	*file;
	size_t	fi;

	resf_cnt = 0;
	fi = 0;
	while (fi < file_cnt)
	{
		file = files[fi];
		check_file(res_files, mask, files, &resf_cnt);
		++fi;
	}
	res_files[resf_cnt] = NULL;
}

void	wc_check_file(char **res_files, char *mask, char *file, size_t *resf_cnt)
{
	bool	f_cancel;
	size_t	mi;
	
	mi = 0;
	f_cancel = false;
	wc_check_file_loop(file, mask, &f_cancel, &mi);
	if (mi < ft_strlen(mask))
		f_cancel = true;
	if (!f_cancel)
	{
		res_files[*resf_cnt] = ft_strdup(file);
		++(*resf_cnt);
	}
}

void	wc_check_file_loop(char *file, char *mask, bool *f_cancel, size_t *mi)
{
	size_t	ci;

	ci = 0;
	while (ci < ft_strlen(file))
	{
		if (*mi == ft_strlen(mask))
		{
			if (ci < ft_strlen(file))
				*f_cancel = true;
			break ;
		}
		if (mask[*mi] == '*')
		{
			if (*mi + 1 == ft_strlen(mask))
			{
				++(*mi);
				break ;
			}
			while (ci < ft_strlen(file))
			{
				if (file[ci] == mask[*mi + 1])
				{
					++(*mi);
					break ;
				}
				++ci;
			}
			--ci;
		}
		else
		{
			if (file[ci] != mask[*mi])
			{
				*f_cancel = true;
				break ;
			}
			else
				++(*mi);
		}
		++ci;
	}
}
