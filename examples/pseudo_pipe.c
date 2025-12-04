#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// ls | grep "a"

int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (*str != '\0')
	{
		len++;
		str++;
	}
}

void	error_msg(void)
{
	const char	*msg_error;

	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

int	main(void)
{
	int	pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	char	*argv_fd1[] = {"/bin/ls", NULL};
	char	*envp[] = {"TERM=xterm", "PATH=/bin:/usr/bin", NULL};
	char	*argv_fd0[] = {"/bin/grep", "a", NULL};

	if (pipe(pipe_fd) == -1)
	{
		error_msg();
		return (1);
	}

	// --- Hijo 1: fd[1] (write)---
	pid1 = fork();
	if (pid1 == -1)
	{
		error_msg();
		return (1);
	}
	if (pid1 == 0)
	{
		// Cerramos el que no usamos
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			error_msg();
			return (1);
		}
		// Lo cerramos, ya tenemos la copia
		close(pipe_fd[1]);
		if (execve("/bin/ls", argv_fd1, envp) == -1)
		{
			error_msg();
			return (1);
		}
	}
	// --- Hijo 2: fd[0] (read)---
	pid2 = fork();
	if (pid2 == -1)
	{
		error_msg();
		return (1);
	}
	if (pid2 == 0)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		{
			error_msg();
			return (1);
		}
		close(pipe_fd[0]);
		if (execve("/bin/grep", argv_fd0, envp) == -1)
		{
			error_msg();
			return (1);
		}
	}

	// --- Padre ---
	// El padre aun tiene ambos fds abierto por la herencia
	// Debemos cerrarlos para que el kernel sepa que no se va a "escribir/leer"
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
