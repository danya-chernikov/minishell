/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:42:08 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 19:50:49 by jhvalenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "prompt_parser.h"
#include "shell.h"
#include "signals.h"
#include "aux_io.h"

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <termios.h>
#include <readline/readline.h>

#include "libft.h"
#include "vector.h"
#include "error.h"

/* Main function in this module.
 * Asks user for all heredocs.
 *     rc - return code */
int	read_heredocs(t_shell *msh)
{
	t_parser_data	*pd;
	size_t			op_i;
	size_t			red_i;
	int				rc;

	op_i = 0;
	pd = msh->pd;
	rc = COMMON_SUCCESS;
	while (op_i < pd->op_cnt)
	{
		red_i = 0;
		while (red_i < pd->ops[op_i].red_cnt)
		{
			if (pd->ops[op_i].redirs[red_i].type == REDIR_HEREDOC)
			{
				rc = read_one_heredoc(msh, &pd->ops[op_i].redirs[red_i].hd);
				if (rc != COMMON_SUCCESS)
					return (rc);
			}
			++red_i;
		}
		++op_i;
	}
	return (rc);
}

static void	hd_init_ctx(t_hd_ctx *ctx)
{
	ctx->status = 0;
	ctx->ret_code = COMMON_SUCCESS;
	ctx->f_have_tty = false;
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &ctx->saved) == 0)
		ctx->f_have_tty = true;
}

static int	hd_finish(t_hd_ctx *ctx)
{
	parent_restore_signals(&ctx->old_int, &ctx->old_quit);
	if (ctx->f_have_tty)
		tcsetattr(STDIN_FILENO, TCSANOW, &ctx->saved);
	if (WIFEXITED(ctx->status)
		&& WEXITSTATUS(ctx->status) == (SIGNALED_CODE + SIGINT))
		return (SIGNALED_CODE + SIGINT);
	if (WIFSIGNALED(ctx->status) && WTERMSIG(ctx->status) == SIGINT)
		return (SIGNALED_CODE + SIGINT);
	return (ctx->ret_code);
}

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

static int	hd_fork_error(t_hd_ctx *ctx, int p[2])
{
	perror("fork");
	close(p[HD_READ]);
	close(p[HD_WRITE]);
	parent_restore_signals(&ctx->old_int, &ctx->old_quit);
	if (ctx->f_have_tty)
		tcsetattr(STDIN_FILENO, TCSANOW, &ctx->saved);
	return (COMMON_SYS_ERR);
}

void	hd_expand_var(t_shell *msh, t_vector *out, const char *line,
			size_t *i)
{
	char	var_name[MAX_ENV_VAL_LEN];
	size_t	j;
	char	*val;

	j = 0;
	++(*i);
	while (line[*i] != '\0' && is_varname_symbol_permitted(line[*i]))
	{
		if (j < MAX_ENV_VAL_LEN - 1)
			var_name[j++] = line[*i];
		++(*i);
	}
	var_name[j] = '\0';
	val = env_get_val(&msh->env, var_name);
	if (!val)
		val = "";
	j = 0;
	while (val[j] != '\0')
	{
		vector_push_back_char(out, val[j]);
		++j;
	}
}

void	hd_expand_special(t_shell *msh, t_vector *out,
			const char *line, size_t *i)
{
	char	var_name[2];
	char	*val;
	size_t	j;

	var_name[0] = line[*i + 1];
	var_name[1] = '\0';
	*i += 2;
	val = env_get_val(&msh->env, var_name);
	if (!val)
		val = "";
	j = 0;
	while (val[j] != '\0')
	{
		vector_push_back_char(out, val[j]);
		++j;
	}
}

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

static void	hd_build_vector(t_shell *msh, t_vector *out, const char *line)
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

static char	*hd_read_line(t_shell *msh, int wfd)
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

static int	hd_collect_chunk(t_heredoc *hd, const char *buf, ssize_t rlen)
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
