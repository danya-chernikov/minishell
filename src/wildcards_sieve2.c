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
		res_files[*resf_cnt] = ft_strdup(wcd->mask);
		if (!res_files[*resf_cnt])
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++(*resf_cnt);
	}
	return (COMMON_SUCCESS);
}
