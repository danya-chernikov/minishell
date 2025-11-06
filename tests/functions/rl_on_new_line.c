/* minishell/tests/functions/rl_on_new_line.c */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define EXIT_CMD	"exit"

/* A handler that prints something
 * without using rl_on_new_line() */
static void	sigquit_handler(int signo)
{
	printf("Caught SIGQUIT\n"); // No tenemos aqui rl_on_new_line()
}

/* A handler that prints something and
 * then calls rl_on_new_line() */
static void	sigint_handler(int signo)
{
	printf("Caught SIGINT\n");
	rl_on_new_line();
}

int	main(void)
{
	char	prompt[] = "dchernik@c3r3s6:~$ ";
	char	*rline_buf = NULL;

	if (signal(SIGQUIT, sigquit_handler) == SIG_ERR)
	{
		fprintf(stderr, "Cannot handle SIGQUIT\n");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		fprintf(stderr, "Cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		rline_buf = readline(prompt);
		printf("rline_buf = %p\n", rline_buf);
		if (strlen(rline_buf) == 0)
		{
			printf("rline_buf is NULL!\n");
			free(rline_buf);
			rline_buf = NULL;
			continue;
		}
		add_history(rline_buf);
		if (!strncmp(rline_buf, EXIT_CMD, strlen(EXIT_CMD)))
		{
			free(rline_buf);
			rline_buf = NULL;
			break;
		}
		free(rline_buf);
		rline_buf = NULL;
	}
	return 0;
}
