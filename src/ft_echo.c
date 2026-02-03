#include "builtin.h"
/*
static size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str != '\0')
	{
		len++;
		str++;
	}
	return (len);
}
*/
static int	is_n_flag(char *arg)
{
	int	index;

	if (arg == NULL || arg[0] != '-' || arg[1] == '\0')
		return (0);
	index = 1;
	while (arg[index] != '\0')
	{
		if (arg[index] != 'n')
			return (0);
		index++;
	}
	return (1);
}

int	ft_echo(char **args)
{
	int	index;
	int	n_flag;

	index = 1;
	n_flag = 0;
	while ((args[index] != NULL) && (is_n_flag(args[index]) == 1))
	{
		n_flag = 1;
		index++;
	}
	while (args[index] != NULL)
	{
		write(STDOUT_FILENO, args[index], ft_strlen(args[index]));
		if (args[index + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		index++;
	}
	if (n_flag == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
