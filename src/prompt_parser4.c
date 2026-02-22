#include "prompt_parser.h"

bool	its_logical_AND(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '&' && prompt[pi + 1] == '&')
		return (true);
	return (false);
}

bool	its_logical_OR(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '|' && prompt[pi + 1] == '|')
		return (true);
	return (false);
}

bool	its_PIPE(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '|' && prompt[pi + 1] != '|')
		return (true);
	return (false);	
}
