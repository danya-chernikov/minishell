#include "exec.h"
#include "aux_io.h"
#include "operand.h"

#include "error.h"

#include <unistd.h>
#include <fcntl.h>

static int	open_heredoc(t_redir *r, int *out_fd);

int	apply_redirs(t_operand *op)
{
	size_t	ri;
	int		fd;
	int		fret;

	ri = 0;
	while (ri < op->red_cnt)
	{
		fret = redir_open_fd(&op->redirs[ri], &fd);
		if (fret == COMMON_SYS_ERR)
			return (COMMON_SYS_ERR);
		if (fret == COMMON_FAILURE)
		{
			print_sys_error(op->redirs[ri].path);
			return (COMMON_FAILURE);
		}
		if (dup2(fd, op->redirs[ri].target_fd) == -1)
			return (COMMON_SYS_ERR);
		if (close(fd) == -1)
			return (COMMON_SYS_ERR);
		++ri;
	}
	return (COMMON_SUCCESS);
}

int	redir_open_fd(t_redir *redir, int *out_fd)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_IN)
		fd = open(redir->path, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APP)
		fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == REDIR_HEREDOC)
		return (open_heredoc(redir, out_fd));
	if (fd == -1)
		return (COMMON_SYS_ERR);
	*out_fd = fd;
	return (COMMON_SUCCESS);
}

static int	open_heredoc(t_redir *r, int *out_fd)
{
	int	p[2];
	int	fret;

	if (pipe(p) == -1)
		return (COMMON_SYS_ERR);
	if (r->hd.content)
	{
		fret = write_all(p[WRITE_END], r->hd.content, r->hd.cnt_len);
		if (fret != COMMON_SUCCESS)
		{
			close(p[READ_END]);
			close(p[WRITE_END]);
			return (COMMON_SYS_ERR);
		}
	}
	if (close(p[WRITE_END]) == -1)
	{
		close(p[READ_END]);
		return (COMMON_SYS_ERR);
	}
	*out_fd = p[READ_END];
	return (COMMON_SUCCESS);
}
