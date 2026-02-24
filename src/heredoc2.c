#include "heredoc.h"
#include "shell.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

/* Expands only $-variables for heredoc body. Quotes
 * inside heredoc body do NOT disable expansion */
char	*heredoc_expand_line(t_shell *msh, const char *line)
{
	t_vector	v;
	size_t		i;

	if (!line)
		return (NULL);
	if (!vector_init(&v, CHAR, ft_strlen(line) + 1))
		return (NULL);

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			const char	*val;
			char		name[MAX_ENV_VAL_LEN];
			size_t		j;

			if (line[i + 1] == '\0')
			{
				vector_push_back_char(&v, '$');
				break ;
			}

			// One-char params
			if (line[i + 1] == '$' || line[i + 1] == '?' ||
				line[i + 1] == '#' || line[i + 1] == '*')
			{
				name[0] = line[i + 1];
				name[1] = '\0';
				val = env_get_val(&msh->env, name);
				if (!val)
					val = "";
				vec_push_str(&v, val);
				i += 2;
				continue ;
			}

			// If next char can't start varname -> literal '$'
			if (!is_varname_symbol_permitted(line[i + 1]))
			{
				vector_push_back_char(&v, '$');
				++i;
				continue ;
			}

			// Read varname
			j = 0;
			i = i + 1;
			while (line[i] && is_varname_symbol_permitted(line[i]))
			{
				if (j < MAX_ENV_VAL_LEN - 1)
					name[j++] = line[i];
				++i;
			}
			name[j] = '\0';

			val = env_get_val(&msh->env, name);
			if (!val)
				val = "";
			vec_push_str(&v, val);
			continue ; // `i` already points to first non-var char
		}
		vector_push_back_char(&v, line[i]);
		++i;
	}
	vector_push_back_char(&v, '\0');

	char *res = ft_strdup((char *)v.data);
	vector_free(&v);
	return (res);
}
