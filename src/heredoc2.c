#include "heredoc.h"
#include "operand.h"
#include "signals.h"
#include "shell.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <sys/wait.h>

void	hd_restore_parent(bool flags[2], struct termios *saved,
			struct sigaction old[2])
{
	if (flags[HD_RESTORE])
		parent_restore_signals(&old[0], &old[1]);
	if (flags[HD_HAVE_TTY])
		tcsetattr(STDIN_FILENO, TCSANOW, saved);
}

int	hd_wait_child(pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
		return (perror("waitpid"), COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

int	hd_finish_status(int status, int ret_code)
{
	if (WIFEXITED(status)
		&& WEXITSTATUS(status) == (SIGNALED_CODE + SIGINT))
	{
		return (SIGNALED_CODE + SIGINT);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (SIGNALED_CODE + SIGINT);
	return (ret_code);
}

void	heredoc_child_loop(t_shell *msh, int wfd, const t_heredoc *hd)
{
	char	*line;
	int		rc;

	g_got_sigint = 0;
	child_set_heredoc_signals();
	while (1)
	{
		line = readline(env_get_val(&msh->env, "PS2"));
		rc = hd_handle_line(msh, wfd, hd, line);
		if (rc == BREAK)
			break ;
		if (rc == (SIGNALED_CODE + SIGINT))
			exit(rc);
	}
	close(wfd);
	exit(EXIT_SUCCESS);
}

int	hd_handle_line(t_shell *msh, int wfd,
		const t_heredoc *hd, char *line)
{
	if (g_got_sigint)
	{
		free(line);
		close(wfd);
		return (SIGNALED_CODE + SIGINT);
	}
	if (!line)
		return (print_shell_warning(NULL, HEREDOC_EOF_WARN_MSG), BREAK);
	if (strings_equal(line, hd->delim))
	{
		free(line);
		return (BREAK);
	}
	return (hd_write_line(msh, wfd, hd, line));
}
