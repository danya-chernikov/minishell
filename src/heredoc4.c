/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:41:38 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 03:58:32 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "shell.h"
#include "signals.h"
#include "aux_io.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	heredoc_child_loop(t_shell *msh, int wfd, const t_heredoc *hd)
{
	char	*line;
	int		rc;

	g_got_sigint = 0;
	child_set_heredoc_signals();
	while (1)
	{
		line = hd_read_line(msh, wfd);
		if (!line)
			break ;
		if (strings_equal(line, hd->delim))
		{
			free(line);
			break ;
		}
		if (!hd->f_expand_body)
			rc = hd_write_raw(wfd, line);
		else
			rc = hd_write_expanded(msh, wfd, line);
		if (rc != COMMON_SUCCESS)
			break ;
	}
	close(wfd);
	exit(EXIT_SUCCESS);
}

char	*hd_read_line(t_shell *msh, int wfd)
{
	char	*line;

	line = readline(env_get_val(&msh->env, "PS2"));
	if (g_got_sigint)
	{
		free(line);
		close(wfd);
		exit(SIGNALED_CODE + SIGINT);
	}
	if (!line)
		print_shell_warning(NULL, HEREDOC_EOF_WARN_MSG);
	return (line);
}

int	hd_write_expanded(t_shell *msh, int wfd, char *line)
{
	t_vector	out;

	if (!vector_init(&out, CHAR, ft_strlen(line) + 1))
	{
		perror("malloc");
		free(line);
		close(wfd);
		exit(EXIT_FAILURE);
	}
	hd_build_vector(msh, &out, line);
	free(line);
	if (write_all(wfd, (char *)out.data, ft_strlen((char *)out.data))
		!= COMMON_SUCCESS
		|| write_all(wfd, "\n", 1) != COMMON_SUCCESS)
	{
		vector_free(&out);
		return (COMMON_FAILURE);
	}
	vector_free(&out);
	return (COMMON_SUCCESS);
}

void	hd_build_vector(t_shell *msh, t_vector *out, const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
			hd_expand_dollar(msh, out, line, &i);
		else
		{
			vector_push_back_char(out, line[i]);
			++i;
		}
	}
	vector_push_back_char(out, '\0');
}
