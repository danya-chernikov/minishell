#include "builtin.h"

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

static void	error_msg_pwd(const char *cmd)
{
	const char	*msg_error;

	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": ", 2);
	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) != NULL)
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	error_msg_pwd("pwd");
	return (1);
}

