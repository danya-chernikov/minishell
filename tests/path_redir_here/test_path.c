#include "path_finder.h"

static void	error_msg(void)
{
	const char	*msg_error;
	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*error_argc;
	char	*route;

	error_argc = "Uso: <comando> [argumentos...]\n";
	if (argc < 2)
	{
		write(STDERR_FILENO, error_argc, ft_strlen(error_argc));
		return (1);
	}
	route = get_cmd_path(argv[1], envp);
	if (route != NULL)
	{
		printf("¡Encontrado! La ruta es: %s\n", route);
		printf("Ahora intentaré ejecutarlo...\n");
		if (execve(route, &argv[1], envp) == -1)
		{
			error_msg();
			free(route);
			return (1);
		}
	}
	else
		fprintf(stderr, "Error: comando '%s' no encontrado en el PATH\n", argv[1]);
	return (0);
}
