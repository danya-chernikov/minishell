#include "aux_prompt_parser.h"

void	remove_right_spaces(char *prompt)
{
	int	i;

	i = ft_strlen(prompt) - 1;
	if (prompt[i] == ' ')
	{
		while (i >= 0 && prompt[i] == ' ')
		{
			prompt[i] = '\0';
			--i;
		}
	}
}

/* Checks for existance of empty parentheses.
 * Sequences like: (), (( )), (((  ))), and etc.
 * Returns true if there are no empty sequences*/
bool	check_empty_par(char *prompt)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(prompt))
	{
		if (prompt[i] == '(' )
		{
			++i;
			skip_spaces(prompt, &i);
			if (i == ft_strlen(prompt)) // Parsing error
				return false;
			if (prompt[i] == ')')
				return false;
		}
		++i;
	}
	return true;
}

/* Checks whether there is an opening parenthesis
 * later to the right, starting from index ind,
 * ignoring only spaces. If it finds '(' while
 * moving to the right and skipping all spaces,
 * it returns its index. Otherwise, it returns -1.
 * If it encounters any symbol other than a space
 * before finding '(', it also returns -1 */
int	later_goes_open_par(char *str, size_t ind)
{
	size_t	slen;

	++ind;
	slen = ft_strlen(str);
	while (ind < slen)
	{
		if (str[ind] == '(')
			return ind;
		if (str[ind] == ' ')
			++ind;
		else
			return -1;
	}
	return (-1);
}

void	skip_spaces(char *prompt, size_t *pi)
{
	while (prompt[*pi] == ' ' && *pi < ft_strlen(prompt))
		++(*pi);
}
