/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:48:44 by dchernik          #+#    #+#             */
/*   Updated: 2026/01/12 15:03:12 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

int	gnl_init(char *buf, char **line, long long *v, int *flags)
{
	if (flags[EXIT])
		return (BREAK);
	if (flags[READ])
	{
		if (v[RLEN] == BUFFER_SIZE && !flags[AGAIN] && buf[v[RLEN] - 1] != '\n')
			flags[EXIT] = 1;
		flags[AGAIN] = 0;
		v[BUF_POS] = 0;
		v[RLEN] = 0;
		v[RLEN] = read(v[FD], buf, BUFFER_SIZE);
		if (v[RLEN] <= 0)
		{
			if (v[RLEN] == -1)
				v[ERR] = 1;
			if (flags[EXIT])
			{
				(*line)[v[LINE_POS] - v[LINE_LEN] + v[I]] = '\0';
				return (RET);
			}
			return (BREAK);
		}
		flags[EXIT] = 0;
	}
	return (NORM);
}

int	gnl_alloc_mem(char **line, long long *v, int *flags)
{
	if (!flags[ALLOC])
	{
		*line = (char *)malloc((v[LINE_LEN] + 2) * sizeof (char));
		v[PRIV_MEM_S] = (v[LINE_LEN] + 2) * sizeof (char);
	}
	else
	{
		*line = (char *)ft_realloc(*line,
				v[PRIV_MEM_S], (v[LINE_POS] + 2) * sizeof (char));
		v[PRIV_MEM_S] = (v[LINE_POS] + 2) * sizeof (char);
	}
	if (*line == NULL)
	{
		v[ERR] = 1;
		return (BREAK);
	}
	return (NORM);
}

void	gnl_check_reaching_end(long long *v, int *flags)
{
	if (v[BUF_POS] == v[RLEN])
	{
		v[BUF_POS]--;
		flags[END] = 1;
	}
}

/* Inner interface */
void	gnl_clear_func_state(char **line, long long *v, int *flags)
{
	*line = NULL;
	v[BUF_POS] = 0;
	v[LINE_POS] = 0;
	v[RLEN] = 0;
	v[I] = 0;
	flags[EXIT] = 0;
	flags[READ] = 1;
	flags[ALLOC] = 0;
	flags[END] = 0;
	flags[AGAIN] = 0;
}

/* Outer interface. Used when we need
 * to stop get_next_line() for some
 * reason. However, get_next_line()
 * must always finish reading the entire
 * line from the file descriptor `fd`.
 * Therefore, the caller must use this
 * function if it decides to stop execution,
 * so that gnl_clear_func_state() is called
 * to ensure the correct functioning of all
 * subsequent get_next_line() calls */
void	gnl_finish(int fd)
{
	char	*line;
	int		err;

	line = get_next_line(fd, &err);
	while (line)
	{
		line = get_next_line(fd, &err);
		free(line);
	}
}
