#include "heredoc.h"
#include "operand.h"
#include "shell.h"
#include "aux_io.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

int	hd_write_line(t_shell *msh, int wfd,
		const t_heredoc *hd, char *line)
{
	char	*out;
	int		rc;

	out = line;
	if (hd->f_expand_body)
	{
		out = heredoc_expand_line(msh, line);
		if (!out)
		{
			perror("malloc");
			free(line);
			close(wfd);
			exit(EXIT_FAILURE);
		}
	}
	rc = hd_write_out(wfd, out);
	if (hd->f_expand_body)
		free(out);
	free(line);
	if (rc != COMMON_SUCCESS)
		return (BREAK);
	return (COMMON_SUCCESS);
}

int	hd_write_out(int wfd, const char *out)
{
	if (write_all(wfd, out, ft_strlen(out)) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	if (write_all(wfd, "\n", 1) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
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
		if (hd_append_buf(hd, buf, rlen) != COMMON_SUCCESS)
			return (close(rfd), COMMON_FAILURE);
		rlen = read(rfd, buf, sizeof(buf));
	}
	if (close(rfd) == -1)
		return (perror("close"), COMMON_SYS_ERR);
	if (rlen < 0)
		return (perror("read"), COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

int	hd_append_buf(t_heredoc *hd, char *buf, ssize_t rlen)
{
	if (hd->cnt_len + (size_t)rlen >= (MAX_HD_CONTENT_LEN - 1))
	{
		print_shell_error(NULL, TOO_LONG_HD_CONTENT);
		return (COMMON_FAILURE);
	}
	hd_copy_buf(hd, buf, (size_t)rlen);
	hd->cnt_len += (size_t)rlen;
	hd->content[hd->cnt_len] = '\0';
	return (COMMON_SUCCESS);
}

void	hd_copy_buf(t_heredoc *hd, char *buf, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		hd->content[hd->cnt_len + i] = buf[i];
		++i;
	}
}
