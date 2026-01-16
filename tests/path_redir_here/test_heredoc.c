#include "heredoc.h"

static void	error_msg(void)
{
	const char	*msg_error;

	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

int	main(int argc, char **argv)
{
	int	fd_heredoc;
	char	buffer[1024];
	int	bytes_read;

	if (argc < 2)
	{
		printf("Uso: [ejecutable] <DELIMITADOR>\n");
		return (1);
	}
	printf("--- Iniciando Heredoc (Escribe '%s' para terminar) ---\n", argv[1]);
	fd_heredoc = get_heredoc_fd(argv[1]);
	if (fd_heredoc == -1)
		return (1);
	printf("\n--- Heredoc terminado. Leyendo contenido del archivo temporal: ---\n");
	bytes_read = read(fd_heredoc, buffer, 1023);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
	}
	else if (bytes_read == 0)
		printf("Fin de Archivo (EOF) detectado.\n");
	else
		error_msg();
	if (close(fd_heredoc) == -1)
	{
		error_msg();
		return (-1);
	}
	return (0);
}
