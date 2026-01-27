#include "heredoc.h"
#include "prompt_parser.h"
#include "signals.h"
#include "aux_io.h"

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <readline/readline.h>

/* Main function in this module.
 * Asks user for all heredocs */
int	read_heredocs(t_parser_data *d)
{
	size_t	op_i;
	size_t	red_i;
	int		ret_code;

	op_i = 0;
	ret_code = COMMON_SUCCESS;
	while (op_i < d->op_cnt)
	{
		red_i = 0;
		while (red_i < d->ops[op_i].red_cnt)
		{
			if (d->ops[op_i].redirs[red_i].type == REDIR_HEREDOC)
			{
				ret_code = read_one_heredoc(&d->ops[op_i].redirs[red_i].hd);
				if (ret_code != COMMON_SUCCESS)
					return (ret_code);
			}
			++red_i;
		}
		++op_i;
	}
	return (ret_code);
}

int	read_one_heredoc(t_heredoc *hd)
{
	int					p[2];
	int					ret_code;
	pid_t				pid;
	int					status;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	if (pipe(p) != 0)
		return (COMMON_SYS_ERR);
	parent_ignore_sigint_sigquit(&old_int, &old_quit);
	pid = fork();
	if (pid == 0)
	{
		close(p[HD_READ]);
		heredoc_child_loop(p[HD_WRITE], hd);
	}
	close(p[HD_WRITE]);
	ret_code = heredoc_parent_collect(p[HD_READ], hd);
	waitpid(pid, &status, 0);
	parent_restore_signals(&old_int, &old_quit);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (SIGNALED_CODE + SIGINT); // 128 + 2
	if (ret_code != COMMON_SUCCESS)
		return (ret_code);
	return (COMMON_SUCCESS);
}

void	heredoc_child_loop(int wfd, const t_heredoc *hd)
{
	char	*line;
	size_t	len;

	child_set_heredoc_signals();
	while (1)
	{
		line = readline("> "); // Add here PS2
		if (!line)
		{
			print_shell_warning(NULL, HEREDOC_EOF_WARN_MSG);
			break ;
		}
		if (strings_equal(line, hd->delim))
		{
			free(line);
			break ;
		}
		// Here we can apply expansion of variables
		// to line in case f_expand_body was set
		len = ft_strlen(line);
		if (write_all(wfd, line, len) != COMMON_SUCCESS ||
			write_all(wfd, "\n", 1) != COMMON_SUCCESS)
		{
			free(line);
			break ;
		}
		free(line);
	}
	close(wfd);
	exit(0);
}

/* Parent reads pipe into hd->content */
int	heredoc_parent_collect(int rfd, t_heredoc *hd)
{
	char	buf[4096];
	ssize_t	rlen;
	size_t	i;

	hd->cnt_len = 0;
	if (hd->content)
		hd->content[0] = '\0';
	rlen = read(rfd, buf, sizeof(buf));
	while (rlen > 0)
	{
		if (hd->cnt_len + (size_t)rlen >= (MAX_HD_CONTENT_LEN - 1))
		{
			print_shell_error(NULL, TOO_LONG_HD_CONTENT);
			close(rfd);
			return (COMMON_FAILURE);
		}
		// Copying read content
		i = 0;	
		while (i < (size_t)rlen)
		{
			hd->content[hd->cnt_len + i] = buf[i];
			++i;
		}
		hd->cnt_len += (size_t)rlen;
		hd->content[hd->cnt_len] = '\0';
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
