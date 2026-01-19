#include "libft.h"
#include <stdlib.h>

void	split_free(char ***res)
{
	size_t	i;

	i = 0;
	while ((*res)[i])
	{
		free((*res)[i]);
		++i;
	}
	free(*res);
}

size_t	split_size(char **res)
{
	size_t	i;

	i = 0;
	while (res[i])
		++i;
	return (i);
}
