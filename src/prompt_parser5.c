#include "prompt_parser.h"

void	skip_spaces(char *prompt, size_t *pi)
{
	while (prompt[*pi] == ' ' && *pi < ft_strlen(prompt))
		++(*pi);
}

bool	is_special_char(char sym)
{
	if (sym == '&' || sym == '|' || sym == '(' || sym == ')')
		return (true);
	return (false);
}

bool	is_inside_quotes(t_parser_data *d, size_t pi)
{
	size_t	qi;
	
	qi = 0;
	while (qi < d->qpair_cnt)
	{
		if (pi > d->quotes[qi].li && pi < d->quotes[qi].ri)
			return (true);
		++qi;
	}
	return (false);
}

bool	is_special_char_outside_quotes(t_parser_data *d, size_t pi)
{
	if (!is_special_char(d->prompt[pi]))
		return (false);
	if (is_inside_quotes(d, pi))
		return (false);
	return (true);
}

void	remove_right_spaces(char *prompt)
{
	int	i;

    if (!prompt || prompt[0] == '\0')
	{
        return ;
	}
	i = (int)ft_strlen(prompt) - 1;
	if (prompt[i] == ' ')
	{
		while (i >= 0 && prompt[i] == ' ')
		{
			prompt[i] = '\0';
			--i;
		}
	}
}

void	remove_left_spaces(char *prompt)
{
	int	i;
	int	j;
	
	if (!prompt || prompt[0] == '\0')
	{
		return ;
	}
	j = 0;
	while (prompt[j] == ' ')
		++j;
	if (j == 0) // No leading spaces
		return ;
	i = 0;
	while (prompt[j] != '\0')
		prompt[i++] = prompt[j++];
	prompt[i] = '\0';
}
