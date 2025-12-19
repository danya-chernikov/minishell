#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#define EXIT_CMD "exit"

/* Common algorithm:
 * If we encounter with a logical operators they run consequently
 * because shell must know the return code of previously executed
 * command to deside if run the next one or not.
 * For example: a && b
 *     First run a (get its return code)
 *     Then run b (get its return code)
 * 
 * If we encounter with pipe, it works differently. All commands
 * inside the pipe are run concurrently. It means, first we should
 * determine where this pipe will be end (what is the last program
 * belonging to this pipe. Now we have to create pipes. The thing
 * is we can create pipes only when we run our programs (cause STDIN_FILENO
 * and STDOUT_FILENO are inner resources of each executable) so now when
 * we reached the end of the current pipeline, we go backwards doing
 * fork(), then creating a pipe for the current command being traversed
 * connecting in such a way its read-end with the write-end  for each pipe operand, 
 * */
int	main(void)
{
	char prompt[] = "dchernik@c3r3s6";
	char *rline_buf = NULL;

	while (1)
	{
		rline_buf = readline(prompt);
		printf("rline_buf = %p\n", rline_buf);
		printf("rline_buf = \"%s\"\n", rline_buf);
		if (strlen(rline_buf) == 0)
		{
			printf("rline_buf is NULL\n");
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

		// Let's analyze received prompt/request
			

		free(rline_buf);
		rline_buf = NULL;
	}
	return 0;
}
