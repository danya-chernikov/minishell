#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "libft.h"

#include "init.h"
#include "engine.h"
#include "builtin.h" // for exit

int	main(int argc, char **argv, char **env)
{
	t_shell	msh;
	int		ret;		// Functions' return code
	int		ret_code;	// Shells' return code

	ret_code = EXIT_SUCCESS;
	ret = msh_init(&msh);
	if (!ret)

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
	return (ret_code);
}
