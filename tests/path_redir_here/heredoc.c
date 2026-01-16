#include "heredoc.h"

static void	error_msg(void)
{
	const char	*msg_error;

	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

int	get_heredoc_fd(char *delimiter)
{
	int	fd;
	char	*line;

	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_msg();
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
			break ;
		}
		if (strncmp(line, delimiter, strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	if (close(fd) == -1)
	{
		error_msg();
		return (-1);
	}
	fd = open(".heredoc_tmp", O_RDONLY);
	if (fd < 0)
	{
		error_msg();
		return (-1);
	}
	if (unlink(".heredoc_tmp") == -1)
	{
		error_msg();
		return (-1);
	}
	return (fd);
}
