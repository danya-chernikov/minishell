#include "heredoc.h"
#include "operand.h"
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
			hd_push_dollar(msh, &v, line, &i);
			continue ;
		}
		vector_push_back_char(&v, line[i]);
		++i;
	}
	return (hd_finish_expand(&v));
}

void	hd_push_dollar(t_shell *msh, t_vector *v,
			const char *line, size_t *i)
{
	char	name[MAX_ENV_VAL_LEN];

	if (line[*i + 1] == '\0')
	{
		vector_push_back_char(v, '$');
		++(*i);
		return ;
	}
	if (line[*i + 1] == '$' || line[*i + 1] == '?'
		|| line[*i + 1] == '#' || line[*i + 1] == '*')
	{
		name[0] = line[*i + 1];
		name[1] = '\0';
		*i += 2;
		return (hd_push_value(msh, v, name));
	}
	if (!is_varname_symbol_permitted(line[*i + 1]))
	{
		vector_push_back_char(v, '$');
		++(*i);
		return ;
	}
	hd_read_name(line, name, i);
	hd_push_value(msh, v, name);
}

void	hd_push_value(t_shell *msh, t_vector *v, char *name)
{
	char	*val;

	val = env_get_val(&msh->env, name);
	if (!val)
		val = "";
	vec_push_str(v, val);
}

void	hd_read_name(const char *line, char *name, size_t *i)
{
	size_t	j;

	j = 0;
	++(*i);
	while (line[*i] && is_varname_symbol_permitted(line[*i]))
	{
		if (j < MAX_ENV_VAL_LEN - 1)
			name[j++] = line[*i];
		++(*i);
	}
	name[j] = '\0';
}

char	*hd_finish_expand(t_vector *v)
{
	char	*res;

	vector_push_back_char(v, '\0');
	res = ft_strdup((char *)v->data);
	vector_free(v);
	return (res);
}
