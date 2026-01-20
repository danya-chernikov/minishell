#include "libft.h"

size_t	ft_abs(long long num)
{
	if (num < 0)
		return (num * -1);
	return (num);
}
