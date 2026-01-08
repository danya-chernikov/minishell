#include "engine.h"

/* If the function returns -1, it means a critical error occurred,
 * and the caller should react by calling exit(EXIT_FAILURE);
 *
 * If it returns 0, a non-critical parser error occurred, and we
 * only need to prompt the user to enter another command in the shell;
 *
 * If it returns 1, everything is fine.
 * The parser_engine() is recursive function!
 * */
int	shell_engine(char *prompt, int *ret_code)
{
	t_parser_data	pdata;

	if (!parser_init(&pdata, prompt)) // Non-critial parser error
		return (0);
	if (!quotes_parser(&pdata))
		return (0);
	if (!parser_engine(&pdata)) // If we got non-critical parser error
		return (0); // Just prompt user to enter another command(s)

	#ifndef DEBUG
		print_all(&pdata);
	#endif

	if (!exec_ops(&pdata, ret_code))
		return (-1);

	// Close all pipes of this prompt
	if (!close_pipes(&pdata))
		return (-1);

	return (1);
}
