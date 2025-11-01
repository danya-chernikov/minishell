#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define EXIT_CMD	"exit"

/* Reads commands from the user until
 * 'exit' command will be received */
int	main(void)
{
	char prompt[] = "dchernik@c3r3s6:~$ ";
	char *rline_buf = NULL;

	while (1)
	{
		rline_buf = readline(prompt);
		rl_on_new_line(); // do we really need it here?
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
	printf("History size before rl_clear_history(): %d\n", history_length);
	rl_clear_history();
	printf("History size after rl_clear_history(): %d\n", history_length);
	return 0;
}
