#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// ls | grep "a" | wc -l
int	main(void)
{
	// P1[0] (read), P1[1] (write) -> ls -> grep
	int	pipe1_fd[2];
	// P2[0] (read), P2[1] (write) -> grep -> wc
	int	pipe2_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	pid_t	pid3;
	char	*msg_error;
	char	*ls_argv[] = {"/bin/ls", NULL};
	char	*grep_argv[] = {"/bin/grep", "a", NULL};
	char	*wc_argv[] = {"/usr/bin/wc", "-l", NULL};
	char	*envp[] = {"TERM=xterm", "PATH=/bin:/usr/bin", NULL};

	if (pipe(pipe1_fd) == -1 || pipe(pipe2_fd) == -1)
	{
		msg_error = strerror(errno);
		write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
		write(STDERR_FILENO, "\n", 1);
		return (1);
	}

	// --- Hijo 1: ls (Escribe a P1) ---
	pid1 = fork();
	if (pid1 == -1)
	{
		msg_error = strerror(errno);
		write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
		write(STDERR_FILENO, "\n", 1);
		return (1);
	}
	if (pid1 == 0)
	{
		// Cerramos el lado de lectura de P1 y ambos de P2 (no usados)
		close(pipe1_fd[0]);
		close(pipe2_fd[0]);
		close(pipe2_fd[1]);
		if (dup2(pipe1_fd[1], STDOUT_FILENO) == -1)
		{
			msg_error = strerror(errno);
			write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
			write(STDERR_FILENO, "\n", 1);
			return (1);
		}
		close(pipe1_fd[1]);
		if (execve("/bin/ls", ls_argv, envp) == -1)
		{
			msg_error = strerror(errno);
			write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
			write(STDERR_FILENO, "\n", 1);
			return (1);
		}
	}
	// --- Hijo 2: grep (Lee de P1, Escribe a P2) ---
	pid2 = fork();
	if (pid2 == -1)
	{
		msg_error = strerror(errno);
		write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
		write(STDERR_FILENO, "\n", 1);
		return (1);
	}
	if (pid2 == 0)
	{
		// Cerramos los extremos no usados
		close(pipe1_fd[1]); // Cerramos escritura P1 (solo lee)
		close(pipe2_fd[0]); // Cerramos lectura P2 (solo escribe)
		if (dup2(pipe1_fd[0], STDIN_FILENO) == -1)
		{
			msg_error = strerror(errno);
			write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
			write(STDERR_FILENO, "\n", 1);
			return (1);
		}
		close(pipe1_fd[0]);
		if (dup2(pipe2_fd[1], STDOUT_FILENO) == -1)
		{
			msg_error = strerror(errno);
			write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
			write(STDERR_FILENO, "\n", 1);
			return (1);
		}
		close(pipe2_fd[1]);
		if (execve("/bin/grep", grep_argv, envp) == -1)
		{
			msg_error = strerror(errno);
			write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
			write(STDERR_FILENO, "\n", 1);
			return (1);
		}
	}
	// --- Hijo 3: wc -l (Lee de P2) ---
	pid3 = fork();
	if (pid3 == -1)
	{
		msg_error = strerror(errno);
		write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
		write(STDERR_FILENO, "\n", 1);
		return (1);
	}
	if (pid3 == 0)
	{
		// Cerramos ambos de P1 y el lado de escritura de P2 que no usamos
		close(pipe1_fd[0]);
		close(pipe1_fd[1]);
		close(pipe2_fd[1]);
		if (dup2(pipe2_fd[0], STDIN_FILENO) == -1)
		{
			msg_error = strerror(errno);
			write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
			write(STDERR_FILENO, "\n", 1);
			return (1);
		}
		close(pipe2_fd[0]);
		if (execve("/usr/bin/wc", wc_argv, envp) == -1)
		{
			msg_error = strerror(errno);
			write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
			write(STDERR_FILENO, "\n", 1);
			return (1);
		}
	}

	// --- Padre ---
	// El Padre debe cerrar todos los descriptores de ambos pipes
	close(pipe1_fd[0]);
	close(pipe1_fd[1]);
	close(pipe2_fd[0]);
	close(pipe2_fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);
	return (0);
}
