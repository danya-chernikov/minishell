#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define PROMPT_LEN	64
#define BUF_LEN		256

/* Shows the difference between fgets() and readline().
 * We just have to call rl_on_new_line(), idk why though..
 * (I mean, without this all seem to work properly) */
int	main(void)
{
	char prompt[PROMPT_LEN] = "dchernik@c3r3s6:~$ ";
	char fgets_buf[BUF_LEN];
	char *rline_buf = NULL;	

	printf("%s", prompt);
	fgets(fgets_buf, BUF_LEN, stdin);
	printf("%s\n", fgets_buf);
	
	rline_buf = readline(prompt);
	rl_on_new_line(); 
	printf("%s\n", rline_buf);
	free(rline_buf);
	return 0;
}
