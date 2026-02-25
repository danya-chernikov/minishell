/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_argredir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 18:09:26 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/26 00:40:01 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "operand.h"
#include "wildcards.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

int	exp_process_argredir(t_shell *msh, t_operand *op,
			t_op_token *op_tok, size_t *opt_i)
{
	int			fret;
	char		**wc_res;
	t_vector	*vec_pair[2];

	if (!exp_vectors_init(vec_pair, ft_strlen(op_tok->cnt) + 1))
		return (COMMON_SYS_ERR);
	exp_expand_argredir_loop(msh, op_tok->cnt, vec_pair);
	wc_collapse_conseq_asterisks(vec_pair[EXP_RES], vec_pair[QMASK]);
	if (wc_alloc_res(&wc_res) != COMMON_SUCCESS)
		return (exp_vectors_free(vec_pair), COMMON_SYS_ERR);
	fret = expand_wildcards(wc_res, (char *)vec_pair[EXP_RES]->data,
			vec_pair[QMASK]);
	if (fret == COMMON_SUCCESS)
	{
		if (*opt_i > 0 && exp_token_is_redirect(&op->tokens[*opt_i - 1]))
		{
			if (!exp_token_is_heredoc(&op->tokens[*opt_i - 1]))
				fret = update_redir_path(op, op_tok, wc_res);
		}
		else
			fret = fill_argv(op, wc_res);
	}
	wc_free_res(&wc_res);
	exp_vectors_free(vec_pair);
	return (fret);
}

/* Updates the redirection path after expansion and wildcard processing.
 * Checks for ambiguous redirects if multiple files match a pattern.*/
int	update_redir_path(t_operand *op, t_op_token *tok, char **wc)
{
	if (tok->redir_ind < 0 || (size_t)tok->redir_ind >= op->red_cnt)
		return (COMMON_SUCCESS);
	if (wc[1][0] != '\0')
	{
		print_shell_error(NULL, AMBIG_REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	free(op->redirs[tok->redir_ind].path);
	op->redirs[tok->redir_ind].path = ft_strdup(wc[0]);
	if (!op->redirs[tok->redir_ind].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/*Fills the operand's argv with results from wildcard expansion.*/
int	fill_argv(t_operand *op, char **wc)
{
	size_t	i;

	i = 0;
	op->f_per_cmd = true;
	while (i < WC_MAX_FILES_NUM && wc[i][0] != '\0')
	{
		if (op->argc >= MAX_ARGC_NUM - 1)
		{
			print_shell_error(NULL, MAX_ARGC_NUM_ERR_MSG);
			return (COMMON_FAILURE);
		}
		if (ft_strlen(wc[i]) >= MAX_ARGV_LEN - 1)
		{
			print_shell_error(NULL, MAX_ARGV_LEN_ERR_MSG);
			return (COMMON_FAILURE);
		}
		op->argv[op->argc] = ft_strdup(wc[i]);
		if (!op->argv[op->argc])
			return (perror("malloc"), COMMON_SYS_ERR);
		op->argc++;
		i++;
	}
	return (COMMON_SUCCESS);
}

/*Iterate over argument/redirection string and expand variables,
 * quotes and tilde, storing the result in vec_pair.*/
void	exp_expand_argredir_loop(t_shell *msh, char *tok_str,
			t_vector *vec_pair[])
{
	size_t		i;
	t_ind_type	state;
	t_exp_ctx	ctx;

	i = 0;
	state = IND_QNONE;
	ctx.msh = msh;
	ctx.tok_str = tok_str;
	ctx.vec_pair = vec_pair;
	ctx.i = &i;
	ctx.state = &state;
	while (tok_str[i])
	{
		exp_handle_char(&ctx);
		i++;
	}
	vector_push_back_char(vec_pair[EXP_RES], '\0');
}

/* Process a single character of tok_str handling quotes,
 * variable expansion, tilde expansion or literal push.*/
void	exp_handle_char(t_exp_ctx *ctx)
{
	char	c;

	c = ctx->tok_str[*ctx->i];
	if (c == '"')
		exp_process_double_quote(ctx->tok_str,
			ctx->i, ctx->vec_pair, ctx->state);
	else if (c == '\'')
		exp_process_single_quote(ctx->tok_str,
			ctx->i, ctx->vec_pair, ctx->state);
	else if (exp_tilde_found_argredir(ctx->tok_str,
			*ctx->i, *ctx->state))
		exp_expand_tilde(ctx->msh,
			ctx->vec_pair, *ctx->state);
	else if (c == '$' && *ctx->state != IND_QSINGLE)
		exp_handle_dollar(ctx);
	else
		push_char_mask(ctx->vec_pair, c, *ctx->state);
}
