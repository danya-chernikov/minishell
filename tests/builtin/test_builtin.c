#include "builtin.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	char *args1[] = {"echo", "hola", "mundo", NULL};
        char *args2[] = {"echo", "-n", "hola", "sin", "salto", NULL};
        char *args3[] = {"echo", "-nnnn", "flag", "multiple", NULL};
	char *cd_args[] = {"cd", "..", NULL};
	char *cd_home[] = {"cd", NULL};

        printf("--- Test PWD ---\n");
        ft_pwd();

        printf("\n--- Test ECHO normal ---\n");
        ft_echo(args1);

        printf("--- Test ECHO flag ---\n");
        ft_echo(args2);
        printf("<-- (El prompt deberia salir pegado aqui)\n");

        printf("\n--- Test ECHO flag raro ---\n");
        ft_echo(args3);
        printf("<-- (El prompt deberia salir pegado aqui)\n");

	printf("\n--- Test CD (Moverse al directorio padre '..') ---\n");	
	printf("Antes: ");
	ft_pwd();

	ft_cd(cd_args, envp);

	printf("Despues: ");
	ft_pwd();

	printf("\n--- Test CD HOME (sin argumentos) ---\n");
	ft_cd(cd_home, envp);
	printf("En casa: ");
	ft_pwd();

	return (0);
}
