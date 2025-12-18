/* minishell/tests/functions/rl_replace_line.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define EXIT_CMD "exit"

static void	sigint_handler(int signo)
{
    char *str = "ya pi";
    char temp[2] = "";
    for (int i = 0; i < strlen(str); ++i)
    {
        temp[0] = str[i];
        rl_replace_line(temp, 0);
        rl_redisplay();
        sleep(1);
    }
}

int main(void)
{
	char prompt[] = "dchernik@c3r3s6:~$ ";
	char *rline_buf = NULL;

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
