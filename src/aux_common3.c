#include "aux_common.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int	perror_and_return(char *where, int ret_code)
{
	perror(where);
	return (ret_code);
}

int	free_one_and_return(void *ptr, int ret_code)
{
	free(ptr);
	return (ret_code);
}

int	free_two_and_return(void *ptr1, void *ptr2, int ret_code)
{
	free(ptr1);
	free(ptr2);
	return (ret_code);
}

/* We don't use it anymore, but let's keep it just in case */
bool	is_surrounded_quotes(char *str)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
		|| (str[0] == '"' && str[ft_strlen(str) - 1] == '"'))
	{
		return (true);
	}
	return (false);
}

/* We don't use it anymore, but let it be here */
void	erase_quotes_no_check(char *str)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	if (!str || len < 2)
		return ;
	while (i < len - 2)
	{
		str[i] = str[i + 1];
		++i;
	}
	str[len - 2] = '\0';
}
