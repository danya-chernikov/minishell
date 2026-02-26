/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:48:12 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/26 03:55:04 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "shell.h"
#include "aux_io.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

void	hd_expand_dollar(t_shell *msh, t_vector *out,
			const char *line, size_t *i)
{
	if (line[*i + 1] == '\0')
	{
		vector_push_back_char(out, '$');
		++(*i);
		return ;
	}
	if (line[*i + 1] == '$' || line[*i + 1] == '?'
		|| line[*i + 1] == '#' || line[*i + 1] == '*')
	{
		hd_expand_special(msh, out, line, i);
		return ;
	}
	if (!is_varname_symbol_permitted(line[*i + 1]))
	{
		vector_push_back_char(out, '$');
		++(*i);
		return ;
	}
	hd_expand_var(msh, out, line, i);
}

int	hd_write_raw(int wfd, char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (write_all(wfd, line, len) != COMMON_SUCCESS
		|| write_all(wfd, "\n", 1) != COMMON_SUCCESS)
	{
		free(line);
		return (COMMON_FAILURE);
	}
	free(line);
	return (COMMON_SUCCESS);
}

/* Expands only $-variables for heredoc body. Quotes
 * inside heredoc body do NOT disable expansion */
char	*heredoc_expand_line(t_shell *msh, const char *line)
{
	t_vector	v;
	size_t		i;
	char		*res;

	if (!line || !vector_init(&v, CHAR, ft_strlen(line) + 1))
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '\0')
			vector_push_back_char(&v, '$');
		else if (line[i] == '$' && !is_varname_symbol_permitted(line[i + 1])
			&& line[i + 1] != '?' && line[i + 1] != '$'
			&& line[i + 1] != '#' && line[i + 1] != '*')
			vector_push_back_char(&v, line[i++]);
		else if (line[i] == '$')
			push_expanded_var(msh, &v, line, &i);
		else
			vector_push_back_char(&v, line[i++]);
	}
	vector_push_back_char(&v, '\0');
	res = ft_strdup((char *)v.data);
	vector_free(&v);
	return (res);
}

/* Extracts a variable name from the line and pushes its corresponding
 * value from the environment into the character vector.*/
void	push_expanded_var(t_shell *msh, t_vector *v, const char *line,
			size_t *i)
{
	char		name[MAX_ENV_VAL_LEN];
	size_t		j;
	const char	*val;

	j = 0;
	(*i)++;
	if (line[*i] == '$' || line[*i] == '?' || line[*i] == '#'
		|| line[*i] == '*')
	{
		name[0] = line[(*i)++];
		name[1] = '\0';
	}
	else
	{
		while (line[*i] && is_varname_symbol_permitted(line[*i])
			&& j < MAX_ENV_VAL_LEN - 1)
			name[j++] = line[(*i)++];
		name[j] = '\0';
	}
	val = env_get_val(&msh->env, name);
	if (!val)
		val = "";
	vec_push_str(v, val);
}
