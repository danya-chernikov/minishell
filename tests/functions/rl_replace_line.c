/* minishell/tests/functions/rl_replace_line.c */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(void)
{
	char	prompt[] = "dchernik@c3r3s6:~$ ";
	char	*rline_buf = NULL;

	rline_buf = readline(prompt);
	printf("You entered: %s\n", rline_buf);
	printf("rl_line_buffer: %s\n", rl_line_buffer);
	free(rline_buf);
	rl_replace_line("New line", 0);
	rl_redisplay();
	rline_buf = readline(prompt);
	printf("You entered: %s\n", rline_buf);
	printf("rl_line_buffer: %s\n", rl_line_buffer);
	free(rline_buf);
	return 0;
}
