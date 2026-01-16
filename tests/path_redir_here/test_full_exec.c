#include "redirect.h"

static void	error_msg(void)
{
	const char	*msg_error;
	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*path;
	char	*error_path;
	char	*args[] = {"ls", "-la", NULL}; // Simulación de argumentos ya parseados
	pid_t	pid;

	(void)argc;
	(void)argv;
	printf("--- SIMULACIÓN: ls -la > output.txt ---\n");
	path = get_cmd_path("ls", envp);
	error_path = "Error: ls no encontrado en PATH\n";
	if (path == NULL)
	{
		write(STDERR_FILENO, error_path, ft_strlen(error_path));
		return (1);
	}
	printf("Path encontrado: %s\n", path);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		printf("Hijo: Redirigiendo salida a 'output.txt'...\n");
		if (redirect_output("output.txt") == -1)
			exit(1);
		// A partir de aquí, printf ya no sale por pantalla, sino al archivo
		// Pero execve reemplaza el proceso, así que ejecutamos ls
		if (execve(path, args, envp) == -1)
		{
			error_msg();
			free(path);
			exit(1);
		}
		exit(1);
	}
	wait(NULL);
	free(path);
	printf("\n--- FIN DE SIMULACIÓN ---\n");
	printf("Verificar el contenido de 'output.txt'. Debería tener el 'ls -la'.\n");
}
