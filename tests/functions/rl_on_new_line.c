/* minishell/tests/functions/rl_on_new_line.c */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_INPUT_LEN	1024
#define EXIT_CMD		"exit"

/* A handler that prints something
 * without using rl_on_new_line() */
static void	sigquit_handler(int signo)
{
	printf("Caught SIGQUIT\n");
}

/* A handler that prints something and
 * then calls rl_on_new_line() */
static void	sigint_handler(int signo)
{
	printf("Caught SIGINT\n");
	rl_on_new_line();
}

/* The best way to avoid output mixing */
static void	sigwinch_handler(int signo)
{
	printf("Caught SIGWINCH\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* Even more stable solution to avoid output mixing */
static void	sigfpe_handler(int signo)
{
	printf("Caught SIGFPE\n");
	exit(1);
}

/* Checks if at least one `symbol` exists in the `str` */
int	symbol_is_present(char *str, char symbol)
{
	for (int i = 0; i < strlen(str); ++i)
	{
		if (str[i] == symbol)
			return 1;
	}
	return 0;
}

/* Finds a simple arithmetic
 * expression (currently only
 * the division operation is
 * supported) in the user's
 * input and calculates it,
 * printing the result */
int	calc(char *line)
{
	char	numerator[MAX_INPUT_LEN];
	char	denominator[MAX_INPUT_LEN];
	int		quotient;
	int		i;

	i = 0;
	while (line[i] != '/' && line[i] != '\0')
	{
		numerator[i] = line[i];
		++i;
	}
	numerator[i] = '\0';
	if ((strlen(numerator) == 0) || (line[i] == '\0'))
	{
		printf("Parser error\n");
		return 0;
	}
	++i;
	while (line[i] != '\0')
	{
		denominator[i - strlen(numerator) - 1] = line[i];
		++i;
	}
	denominator[i - strlen(numerator) - 1] = '\0';
	if (strlen(denominator) == 0)
	{
		printf("Parser error\n");
		return 0;
	}
	quotient = atoi(numerator)/atoi(denominator);
	printf("%d\n", quotient);
	return 1;
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
	if (signal(SIGWINCH, sigwinch_handler) == SIG_ERR)
	{
		fprintf(stderr, "Cannot handle SIGWINCH\n");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGFPE, sigfpe_handler) == SIG_ERR)
	{
		fprintf(stderr, "Cannot handle SIGFPE\n");
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
		if (strlen(rline_buf) > MAX_INPUT_LEN)
		{
			printf("Your input is too long\n");
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
		if (symbol_is_present(rline_buf, '/'))
			calc(rline_buf);
		free(rline_buf);
		rline_buf = NULL;
	}
	return 0;
}
