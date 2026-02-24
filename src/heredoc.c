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

int	read_one_heredoc(t_shell *msh, t_heredoc *hd)
{
	int					p[2];
	int					ret_code;
	pid_t				pid;
	int					status;
	bool				f_have_tty;
	bool				f_restore_signals;
	struct termios		saved;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	status = 0;
	ret_code = COMMON_SUCCESS;
	f_have_tty = false;
	f_restore_signals = false;
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &saved) == 0)
			f_have_tty = 1;
	}
	if (pipe(p) != 0)
	{
		perror("pipe");
		return (COMMON_SYS_ERR);
	}
	parent_ignore_sigint_sigquit(&old_int, &old_quit);
	f_restore_signals = true;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(p[HD_READ]);
		close(p[HD_WRITE]);
		if (f_restore_signals)
			parent_restore_signals(&old_int, &old_quit);
		if (f_have_tty)
			tcsetattr(STDIN_FILENO, TCSANOW, &saved);
		return (COMMON_SYS_ERR);
	}
	else if (pid == 0)
	{
		close(p[HD_READ]);
		heredoc_child_loop(msh, p[HD_WRITE], hd);
		// heredoc_child_loop() has to be terminated via exit()
	}
	close(p[HD_WRITE]);
	ret_code = heredoc_parent_collect(p[HD_READ], hd);
	waitpid(pid, &status, 0);
	if (f_restore_signals)
		parent_restore_signals(&old_int, &old_quit);
	if (f_have_tty)
		tcsetattr(STDIN_FILENO, TCSANOW, &saved);

	if (WIFEXITED(status) && WEXITSTATUS(status) == (SIGNALED_CODE + SIGINT))
		return (SIGNALED_CODE + SIGINT);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (SIGNALED_CODE + SIGINT); // 128 + 2
	return (ret_code);
}

void    heredoc_child_loop(t_shell *msh, int wfd, const t_heredoc *hd)
{
    char    *line;

    g_got_sigint = 0;
    child_set_heredoc_signals();

    while (1)
    {
        line = readline(env_get_val(&msh->env, "PS2"));

        if (g_got_sigint)
        {
            free(line);
            close(wfd);
            exit(SIGNALED_CODE + SIGINT);
        }

        if (!line)
        {
            print_shell_warning(NULL, HEREDOC_EOF_WARN_MSG);
            break;
        }

        if (strings_equal(line, hd->delim))
        {
            free(line);
            break;
        }

        // write line (optionally expanding)
        if (!hd->f_expand_body)
        {
            size_t len = ft_strlen(line);
            if (write_all(wfd, line, len) != COMMON_SUCCESS ||
                write_all(wfd, "\n", 1) != COMMON_SUCCESS)
            {
                free(line);
                break;
            }
            free(line);
            continue;
        }
        else
        {
            t_vector out;
            size_t   i;

            if (!vector_init(&out, CHAR, ft_strlen(line) + 1))
            {
                perror("malloc");
                free(line);
                close(wfd);
                exit(EXIT_FAILURE);
            }

            i = 0;
            while (line[i] != '\0')
            {
                if (line[i] == '$')
                {
                    char    var_name[MAX_ENV_VAL_LEN];
                    size_t  j;
                    char    *val;

                    if (line[i + 1] == '\0')
                    {
                        vector_push_back_char(&out, '$');
                        ++i;
                        continue;
                    }

                    if (line[i + 1] == '$' || line[i + 1] == '?' ||
                        line[i + 1] == '#' || line[i + 1] == '*')
                    {
                        var_name[0] = line[i + 1];
                        var_name[1] = '\0';
                        i += 2;

                        val = env_get_val(&msh->env, var_name);
                        if (!val)
                            val = "";
                        for (j = 0; val[j] != '\0'; ++j)
                            vector_push_back_char(&out, val[j]);
                        continue;
                    }

                    // If next char can't start a name treat '$' as literal
                    if (!is_varname_symbol_permitted(line[i + 1]))
                    {
                        vector_push_back_char(&out, '$');
                        ++i;
                        continue;
                    }

                    // Normal $VARNAME
                    j = 0;
                    ++i; // skip '$'
                    while (line[i] != '\0' && is_varname_symbol_permitted(line[i]))
                    {
                        if (j < MAX_ENV_VAL_LEN - 1)
                            var_name[j++] = line[i];
                        ++i;
                    }
                    var_name[j] = '\0';

                    val = env_get_val(&msh->env, var_name);
                    if (!val)
                        val = "";
                    for (j = 0; val[j] != '\0'; ++j)
                        vector_push_back_char(&out, val[j]);

                    continue; // `i` already points on the first non-symbol of the name 
                }

                vector_push_back_char(&out, line[i]);
                ++i;
            }

            vector_push_back_char(&out, '\0');

            if (write_all(wfd, (char *)out.data, ft_strlen((char *)out.data)) != COMMON_SUCCESS ||
                write_all(wfd, "\n", 1) != COMMON_SUCCESS)
            {
                vector_free(&out);
                free(line);
                break;
            }

            vector_free(&out);
            free(line);
        }
    }

    close(wfd);
    exit(EXIT_SUCCESS);
}

/* Parent reads pipe into hd->content */
int	heredoc_parent_collect(int rfd, t_heredoc *hd)
{
	char	buf[MAX_HD_CONTENT_LEN];
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
