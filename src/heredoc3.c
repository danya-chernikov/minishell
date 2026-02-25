/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:40:58 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 00:40:59 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "shell.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	read_one_heredoc(t_shell *msh, t_heredoc *hd)
{
	t_hd_ctx	ctx;
	int			p[2];
	pid_t		pid;

	hd_init_ctx(&ctx);
	if (pipe(p) != 0)
	{
		perror("pipe");
		return (COMMON_SYS_ERR);
	}
	parent_ignore_sigint_sigquit(&ctx.old_int, &ctx.old_quit);
	pid = fork();
	if (pid < 0)
		return (hd_fork_error(&ctx, p));
	if (pid == 0)
	{
		close(p[HD_READ]);
		heredoc_child_loop(msh, p[HD_WRITE], hd);
	}
	close(p[HD_WRITE]);
	ctx.ret_code = heredoc_parent_collect(p[HD_READ], hd);
	waitpid(pid, &ctx.status, 0);
	return (hd_finish(&ctx));
}

int	hd_fork_error(t_hd_ctx *ctx, int p[2])
{
	perror("fork");
	close(p[HD_READ]);
	close(p[HD_WRITE]);
	parent_restore_signals(&ctx->old_int, &ctx->old_quit);
	if (ctx->f_have_tty)
		tcsetattr(STDIN_FILENO, TCSANOW, &ctx->saved);
	return (COMMON_SYS_ERR);
}

/* Parent reads pipe into hd->content */
int	heredoc_parent_collect(int rfd, t_heredoc *hd)
{
	char	buf[MAX_HD_CONTENT_LEN];
	ssize_t	rlen;

	hd->cnt_len = 0;
	if (hd->content)
		hd->content[0] = '\0';
	rlen = read(rfd, buf, sizeof(buf));
	while (rlen > 0)
	{
		if (hd_collect_chunk(hd, buf, rlen) != COMMON_SUCCESS)
		{
			close(rfd);
			return (COMMON_FAILURE);
		}
		rlen = read(rfd, buf, sizeof(buf));
	}
	close(rfd);
	if (rlen < 0)
	{
		perror("read");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

int	hd_collect_chunk(t_heredoc *hd, const char *buf, ssize_t rlen)
{
	size_t	i;

	if (hd->cnt_len + (size_t)rlen >= (MAX_HD_CONTENT_LEN - 1))
	{
		print_shell_error(NULL, TOO_LONG_HD_CONTENT);
		return (COMMON_FAILURE);
	}
	i = 0;
	while (i < (size_t)rlen)
	{
		hd->content[hd->cnt_len + i] = buf[i];
		++i;
	}
	hd->cnt_len += (size_t)rlen;
	hd->content[hd->cnt_len] = '\0';
	return (COMMON_SUCCESS);
}
