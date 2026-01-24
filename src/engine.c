#include "engine.h"

/* If the function returns -1, it means a critical error occurred,
 * and the caller should react by calling exit(EXIT_FAILURE);
 *
 * If it returns 0, a non-critical parser error occurred, and we
 * only need to prompt the user to enter another command in the shell;
 *
 * If it returns 1, everything is fine.
 * The parser_engine() is recursive function! */
int	shell_engine(char *prompt, int *ret_code)
{
	t_parser_data	pdata;
	int				fres; // Returned code from function
	int				try;
	
	try = 0;
	fres = COMMON_SUCCESS;
	ft_bzero(&pdata, sizeof(pdata));
	while (try < 1)
	{
		if (!prompt || ft_strlen(prompt) == 0)
			break ;

		fres = parser_init(&pdata, prompt);
		if (fres != COMMON_SUCCESS) // Non-critial parser error
			break ;

		fres = quotes_parser(&pdata);
		if (fres != COMMON_SUCCESS)
			break ;

		// May be launched only after quote intervals will be parsed
		fres = comments_parser(&pdata);

		fres = check_empty_par(&pdata);
		if (fres != COMMON_SUCCESS)
			break ;

		fres = parser_engine(&pdata);
		if (fres != COMMON_SUCCESS) // If we got non-critical parser error
			break ; // Just prompt user to enter another command(s)

#if DEBUG == 1
		dbg_prompt_parser_print_all(&pdata);
#endif

		fres = exec_ops(&pdata, ret_code);
		if (fres != COMMON_SUCCESS)
			break ;

		// Close all pipes of this prompt
		fres = close_pipes(&pdata);

		++try;
	} // End try block

	parser_free(&pdata);
	return (fres);
}

/* Finds the first comment symbol #
 * that is located outside any quote
 * interval and truncates `prompt`
 * buffer of `s_parser_data` structure
 * eliminating all what goes after this
 * comment symbol.  May be launched only
 * after quote intervals will be parsed
 * calling quotes_parser() */
int	comments_parser(t_parser_data *d)
{
	size_t	pi;

	pi = 0;
	while (pi < ft_strlen(d->prompt))
	{
		if (d->prompt[pi] == '#' && !is_inside_quotes(d, pi))
		{
			d->prompt[pi] = '\0';
			break ;
		}
		++pi;
	}
	return (COMMON_SUCCESS);
}
