#include "redirect.h"

#if 0
Maneja '<'.
Abre el archivo en modo lectura y lo conecta al STDIN (0).
#endif

static void	error_msg(void)
{
	const char	*msg_error;

	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

int	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		error_msg();
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		error_msg();
		if (close(fd) == -1)
		{
			error_msg();
			return (-1);
		}
		return (-1);
	}
	if (close(fd) == -1)
	{
		error_msg();
		return (-1);
	}
	return (0);
}

#if 0
Maneja '>' (Sobrescribir).
Abre el archivo, lo limpia (TRUNC) y lo conecta al STDOUT (1).
#endif
int	redirect_output(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_msg();
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		error_msg();
		if (close(fd) == -1)
		{
			error_msg();
			return (-1);
		}
		return (-1);
	}
	if (close(fd) == -1)
	{
		error_msg();
		return (-1);
	}
	return (0);
}

#if 0
Maneja '>>' (Añadir).
Abre el archivo, va al final (APPEND) y lo conecta al STDOUT (1).
#endif
int	redirect_append(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		error_msg();
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		error_msg();
		if (close(fd) == -1)
		{
			error_msg();
			return (-1);
		}
		return (-1);
	}
	if (close(fd) == -1)
	{
		error_msg();
		return (-1);
	}
	return (0);
}
