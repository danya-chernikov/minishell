/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:48:41 by dchernik          #+#    #+#             */
/*   Updated: 2026/01/09 23:01:40 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* The `v` array was defined as long long
 * to be able to fit into it both variables
 * of size_t and int types.
 *     v - the array holding variables. */
char	*get_next_line(int fd)
{
	static char			buf[BUFFER_SIZE];
	static char			*line = NULL;
	static int			flags[5] = {0, 1, 0, 0, 0};
	static long long	v[8];

	v[FD] = (long long)fd;
	if (v[BUF_POS] >= v[RLEN] && v[BUF_POS] > 0 && v[RLEN] > 0)
		flags[EXIT] = 1;
	while (1)
	{
		v[RES] = loop_alg(buf, &line, v, flags);
		if (v[RES] == RET)
			return (line);
		else if (v[RES] == BREAK)
			break ;
		else if (v[RES] == CONT)
			continue ;
	}
	clear_func_state(&line, v, flags);
	return (NULL);
}

int	loop_alg(char *buf, char **line, long long *v, int *flags)
{
	int	res;

	res = init(buf, line, v, flags);
	if (res == RET)
		return (RET);
	else if (res == BREAK)
		return (BREAK);
	if (get_chunk(buf, line, v, flags) == BREAK)
		return (BREAK);
	check_reaching_end(v, flags);
	if (buf[v[BUF_POS]] == '\n')
	{
		process_new_line(buf, line, v, flags);
		return (RET);
	}
	else
	{
		if (process_end_chunk(line, v, flags))
			return (RET);
		return (CONT);
	}
	return (NORM);
}

/* It finds the first chunk of data in read buffer,
 * allocates memory for it and copies its content
 * into the `line` */
int	get_chunk(char *buf, char **line, long long *v, int *flags)
{
	v[I] = 0;
	while (buf[v[BUF_POS]] != '\n' && v[BUF_POS] < v[RLEN])
	{
		v[BUF_POS]++;
		v[I]++;
	}
	v[LINE_LEN] = v[I];
	v[LINE_POS] += v[LINE_LEN];
	if (alloc_mem(line, v, flags) == BREAK)
		return (BREAK);
	v[I] = 0;
	while (v[I] < v[LINE_LEN])
	{
		(*line)[v[LINE_POS] - v[LINE_LEN] + v[I]]
			= buf[v[BUF_POS] - v[LINE_LEN] + v[I]];
		v[I]++;
	}
	return (NORM);
}

void	process_new_line(char *buf, char **line, long long *v, int *flags)
{
	(*line)[v[LINE_POS] - v[LINE_LEN] + v[I]] = '\n';
	(*line)[v[LINE_POS] - v[LINE_LEN] + v[I] + 1] = '\0';
	if (v[BUF_POS] < v[RLEN] - 1 && !flags[END])
		v[BUF_POS]++;
	v[LINE_POS] = 0;
	flags[ALLOC] = 0;
	flags[READ] = 0;
	flags[END] = 0;
	if (v[BUF_POS] == v[RLEN] - 1 && buf[v[BUF_POS]] == '\n')
	{
		flags[READ] = 1;
		if (!flags[AGAIN] && buf[v[BUF_POS] - 1] == '\n')
		{
			flags[AGAIN] = 1;
			flags[READ] = 0;
		}
	}
}

int	process_end_chunk(char **line, long long *v, int *flags)
{
	flags[ALLOC] = 1;
	flags[READ] = 1;
	flags[END] = 0;
	if (v[RLEN] < BUFFER_SIZE)
	{
		(*line)[v[LINE_POS] - v[LINE_LEN] + v[I]] = '\0';
		flags[ALLOC] = 0;
		v[LINE_POS] = 0;
		return (RET);
	}
	return (NORM);
}
