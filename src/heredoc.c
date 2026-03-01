/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:05:02 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:05:03 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "prompt_parser.h"
#include "operand.h"
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

static void	read_one_heredoc_init(t_hd_helper *helper, int *status);

/* Main function in this module.
 * Asks user for all heredocs. */
int	read_heredocs(t_shell *msh)
{
	t_parser_data	*pd;
	size_t			op_i;
	int				rc;

	pd = msh->pd;
	op_i = 0;
	rc = COMMON_SUCCESS;
	while (op_i < pd->op_cnt)
	{
		rc = read_one_operand_hd(msh, &pd->ops[op_i]);
		if (rc != COMMON_SUCCESS)
			return (rc);
		++op_i;
	}
	return (rc);
}

int	read_one_operand_hd(t_shell *msh, t_operand *op)
{
	size_t	red_i;
	int		rc;

	red_i = 0;
	rc = COMMON_SUCCESS;
	while (red_i < op->red_cnt)
	{
		if (op->redirs[red_i].type == REDIR_HEREDOC)
		{
			rc = read_one_heredoc(msh, &op->redirs[red_i].hd);
			if (rc != COMMON_SUCCESS)
				return (rc);
		}
		++red_i;
	}
	return (rc);
}

int	read_one_heredoc(t_shell *msh, t_heredoc *hd)
{
	int					p[2];
	int					ret_code;
	int					status;
	struct termios		saved;
	t_hd_helper			helper;

	read_one_heredoc_init(&helper, &status);
	ret_code = hd_prepare_parent(p, helper.flags, &saved, helper.old);
	if (ret_code != COMMON_SUCCESS)
		return (ret_code);
	helper.pid = fork();
	if (helper.pid < 0)
		return (hd_fork_failed(p, helper.flags, &saved, helper.old));
	if (helper.pid == 0)
	{
		close(p[HD_READ]);
		heredoc_child_loop(msh, p[HD_WRITE], hd);
		exit(EXIT_FAILURE);
	}
	close(p[HD_WRITE]);
	ret_code = heredoc_parent_collect(p[HD_READ], hd);
	if (hd_wait_child(helper.pid, &status) != COMMON_SUCCESS)
		ret_code = COMMON_SYS_ERR;
	hd_restore_parent(helper.flags, &saved, helper.old);
	return (hd_finish_status(status, ret_code));
}

static void	read_one_heredoc_init(t_hd_helper *helper, int *status)
{
	helper->flags[HD_HAVE_TTY] = false;
	helper->flags[HD_RESTORE] = false;
	*status = 0;
}

int	hd_prepare_parent(int p[2], bool flags[2],
		struct termios *saved, struct sigaction old[2])
{
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, saved) == 0)
		flags[HD_HAVE_TTY] = true;
	if (pipe(p) != 0)
		return (perror("pipe"), COMMON_SYS_ERR);
	parent_ignore_sigint_sigquit(&old[0], &old[1]);
	flags[HD_RESTORE] = true;
	return (COMMON_SUCCESS);
}
