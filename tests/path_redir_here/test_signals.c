#include "signals.h"

static void	error_msg(void)
{
	const char	*msg_error;

	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

int	main(void)
{
	char	*line;
	pid_t	pid;
	int	status;

	printf("--- TEST DE SEÑALES ---\n");
	printf("PID: %d\n", getpid());
	while (1)
	{
		init_signals_interactive();
		line = readline("minishell$ ");
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strncmp(line, "cat", 3) == 0)
		{
			init_signals_execution();
			pid = fork();
			if (pid == -1)
				error_msg();
			if (pid == 0)
			{
				init_signals_child();
				printf("    [Hijo] Ejecutando 'cat'. Prueba Ctrl+C o Ctrl+\\ ahora.\n");
				if (execlp("cat", "cat", NULL) == -1)
				{
					error_msg();
					exit(1);
				}
				exit(1);
			}
			if (waitpid(pid, &status, 0) == -1)
			{
				error_msg();
				break ;
			}
			printf("\n    [Padre] El hijo ha terminado. Volvemos al prompt.\n");
		}
		free(line);
	}
	return (0);
}
