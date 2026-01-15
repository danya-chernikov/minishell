#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "libft.h"

#include "shell.h"
#include "engine.h"
#include "builtin.h" // for exit

/*
	SUCCESS_CODE = 0,
	SYS_ERR = 1,
	CMD_BUILTIN_ERR = 2,
	SYNTAX_ERR = 2,
	CMD_LOCATED_BUT_NOT_EXEC_ERR = 126,
	CMD_NOT_LOCATED_ERR = 127
 * */
int	main(int argc, char **argv, char **env)
{
	t_shell	msh;
	int		fret;
	int		shret;

	shret = EXIT_SUCCESS;
	fret = msh_init(&msh);
	if (fret == COMMON_SYS_ERR) // -1
		shret = SYS_ERR; // 1
	else
	{
		// One of the other possible errors
		// described in the `e_exit_code`
		// structure, or 128 + N, where N
		// is the number of the signal
		// that interrupted our minishell
		shret = fret;
	}

	// Initialize all variables what
	// are left (local and environmental)
	msh_set_vars(&msh);

	// Read configs
	
	// Execute all things from configs
	
	// Setup history (read lines from history file and add them)
	
	// Depending on our shell mode launch the appropriate engine

	/*
	char	prompt[PROMPT_INV_LEN];
	char	*rline_buf;

	rline_buf = NULL;
	strncpy(prompt, "dchernik@c3r3s6: ", PROMPT_INV_LEN);
	while (1)
	{
		rline_buf = readline(prompt);
		if (ft_strlen(rline_buf) == 0)
		{
			free(rline_buf);
			rline_buf = NULL;
			continue;
		}
		add_history(rline_buf);
		if (!strncmp(rline_buf, EXIT_CMD, ft_strlen(EXIT_CMD)) &&
			ft_strlen(rline_buf) == ft_strlen(EXIT_CMD))
		{
			free(rline_buf);
			rline_buf = NULL;
			break;
		}
		if (shell_engine(rline_buf, &ret_code) == -1) // Critial system error occured
			exit(EXIT_FAILURE);
		// In case if non-critial parser error occured
		// we just free `rline_buf` and prompt user again
		free(rline_buf);
		rline_buf = NULL;
	}*/

	msh_free(&msh);
	return (shret);
}
