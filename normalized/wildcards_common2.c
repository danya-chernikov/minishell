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
