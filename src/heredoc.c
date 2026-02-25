/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:42:08 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 23:20:13 by dchernik         ###   ########.fr       */
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

void	hd_init_ctx(t_hd_ctx *ctx)
{
	ctx->status = 0;
	ctx->ret_code = COMMON_SUCCESS;
	ctx->f_have_tty = false;
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &ctx->saved) == 0)
		ctx->f_have_tty = true;
}

int	hd_finish(t_hd_ctx *ctx)
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
