#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "libft.h"

#include "parser.h"
#include "engine.h"
#include "quote.h"
#include "builtin.h"
#include "debug.h"

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;

	t_parser_data	pdata;
	char			prompt[PROMPT_INV_LEN];
	char			*rline_buf;

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

		// Let's analyze the received prompt/request
		
		if (!parser_init(&pdata, rline_buf))
			continue;

		if (!quotes_parser(&pdata))
			continue;

		print_quotes(&pdata);
		/*
		if (!parser_engine(&pdata)) // If we got non-critical parser error
			continue; // Just prompt user to enter another command(s)

		print_parsed_data(&pdata);
		print_tokens(&pdata);
		print_parentheses(&pdata);

		if (!exec_ops(&pdata))
			exit(EXIT_FAILURE);

        // Close all pipes of this prompt
		if (!close_pipes(&pdata))
			exit(EXIT_FAILURE);*/
			
		free(rline_buf);
		rline_buf = NULL;

	}
	return (0);
}
