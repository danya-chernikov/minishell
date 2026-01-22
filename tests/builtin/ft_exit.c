#include "builtin.h"

static int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

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

static int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + (*str - '0');
		str++;
	}
	return (result * sign);
}

static int	is_numeric(char *str)
{
	int	index;

	index = 0;
	if (str[index] == '+' || str[index] == '-')
		index++;
	if (str[index] == '\0')
		return (0);
	while (str[index] != '\0')
	{
		if (ft_isdigit(str[index]) == 0)
			return (0);
		index++;
	}
	return (1);
}

int	ft_exit(char **args)
{
	char	*error_many_args;
	char	*error_no_nbr;

	write(STDOUT_FILENO, "exit\n", 5);
	if (args[1] == NULL)
		exit(0);
	if (is_numeric(args[1]) != 0)
	{
		if (args[2] != NULL)
		{
			error_many_args = "minishell: exit: too many arguments\n";
			write(STDERR_FILENO, error_many_args, ft_strlen(error_many_args));
			return (1);
		}
		exit((unsigned char)ft_atoi(args[1]));
	}
	else
	{
		error_no_nbr = "minishell: exit: numeric argument required\n";
		write(STDERR_FILENO, error_no_nbr, ft_strlen(error_no_nbr));
		exit(2);
	}
	return (0);
}
