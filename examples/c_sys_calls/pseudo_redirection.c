int	main(void)
{
	int	fd;
	char	*msg_error;

	while (redirection)
	{
		fd = -1;
		if (redirection->type == REDIR_OUT)
			fd = open(redirection->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redirection->type == REDIR_OUT_APPEND)
			fd = open(redirection->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redirection->type == REDIR_IN)
			fd = open(redirection->file, ORDONLY);
		else if (redirection->type == REDIR_HEREDIC)
			// logica de la redireccion <<
		
		if (fd == -1)
		{
			msg_error = strerror(errno);
			write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
			write(STDERR_FILENO, "\n", 1);
			exit(EXIT_FAILURE);
		}

		if (redirection->type == REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);

		close(fd);
		redirection = redirection->next;
	}
