#include "aux_common.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* If ~ does not exist in `str` returns	a duplicate
 * of `str` on heap. On error returns NULL */
char	*expand_homedir(char *str, char *home)
{
	size_t		new_size;
	char		*new_str;
	char		*tilda;
	ptrdiff_t	pdif;
	
	tilda = ft_strchr(str, '~');
	if (!tilda)
		return (ft_strdup(str));
	// Minus one ~ symbol plus one null-byte annihilate each other
	new_size = ft_strlen(str) + ft_strlen(home); // -1 + 1
	new_str = (char *)malloc(new_size * sizeof(char));
	if (!new_str)
		return (NULL);
	// plus 1 null-terminator	
	pdif = (ptrdiff_t)tilda - (ptrdiff_t)str;
	ft_strlcpy(new_str, str, pdif + 1);
	ft_strlcat(new_str, home, new_size);
	ft_strlcat(new_str, str + pdif + 1, new_size);
	return (new_str);
}

bool	contains_quote(char *str)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(str))
	{
		if (str[i] == '\'' || str[i] == '"')
			return (true);
		++i;
	}
	return (false);
}

bool	is_variable_name_correct(char *var_name)
{
	size_t	i;

	if (!var_name || var_name[0] == '\0')
		return (false);

	// First char
	if (!(ft_isalpha(var_name[0]) || var_name[0] == '_'))
		return (false);

	// Remaining chars
	i = 1;
	while (var_name[i] != '\0')
	{
		if (!(ft_isalnum(var_name[i]) || var_name[i] == '_'))
			return (false);
		++i;
	}

	return (true);
}

/* We don't use it anymore, but let's keep it just in case */
bool	is_surrounded_quotes(char *str)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\'') ||
		(str[0] == '"' && str[ft_strlen(str) - 1] == '"'))
	{
		return (true);
	}
	return (false);
}

/* We don't use it anymore, but let it be here */
void	erase_quotes_no_check(char *str)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	if (!str || len < 2)
		return ;
	while (i < len - 2)
	{
		str[i] = str[i + 1];
		++i;
	}
	str[len - 2] = '\0';
}
