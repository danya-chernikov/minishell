/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_argredir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:55:30 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 14:01:58 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "shell.h"
#include "operand.h"
#include "wildcards.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

static int	update_redir_path(t_operand *op, t_op_token *op_tok, char **wc_res);
static int	append_wc_matches(t_operand *op, char **wc_res);
static int	expand_one_field(t_operand *op, t_vector *vec_pair[],
				size_t start, size_t end);
static int	append_split_fields(t_operand *op, t_vector *vec_pair[]);

int	exp_process_argredir(t_shell *msh, t_operand *op,
		t_op_token *op_tok, size_t *opt_i)
{
	int			fret;
	char		**wc_res;
	t_vector	*vec_pair[2];

	if (exp_vectors_init(vec_pair, ft_strlen(op_tok->cnt) + 1)
		!= COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	exp_expand_argredir_loop(msh, op_tok->cnt, vec_pair);
	wc_collapse_conseq_asterisks(vec_pair[EXP_RES], vec_pair[QMASK]);
	if (wc_alloc_res(&wc_res) != COMMON_SUCCESS)
	{
		exp_vectors_free(vec_pair);
		return (COMMON_SYS_ERR);
	}
	if (*opt_i > 0 && exp_token_is_redirect(&op->tokens[*opt_i - 1]))
	{
		if (exp_token_is_heredoc(&op->tokens[*opt_i - 1]))
			fret = COMMON_SUCCESS;
		else
		{
			fret = expand_wildcards(wc_res,
					(char *)vec_pair[EXP_RES]->data, vec_pair[QMASK]);
			if (fret == COMMON_SUCCESS)
				fret = update_redir_path(op, op_tok, wc_res);
		}
	}
	else
		fret = append_split_fields(op, vec_pair);
	wc_free_res(&wc_res);
	exp_vectors_free(vec_pair);
	return (fret);
}

static int	update_redir_path(t_operand *op, t_op_token *op_tok, char **wc_res)
{
	if (op_tok->redir_ind < 0 || (size_t)op_tok->redir_ind >= op->red_cnt)
		return (COMMON_SUCCESS);
	if (wc_res[1][0] != '\0')
	{
		print_shell_error(NULL, AMBIG_REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	free(op->redirs[op_tok->redir_ind].path);
	op->redirs[op_tok->redir_ind].path = ft_strdup(wc_res[0]);
	if (!op->redirs[op_tok->redir_ind].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

static int	append_wc_matches(t_operand *op, char **wc_res)
{
	size_t	i;

	i = 0;
	op->f_per_cmd = true;
	while (i < WC_MAX_FILES_NUM && wc_res[i][0] != '\0')
	{
		if (op->argc >= MAX_ARGC_NUM - 1)
		{
			print_shell_error(NULL, MAX_ARGC_NUM_ERR_MSG);
			return (COMMON_FAILURE);
		}
		if (ft_strlen(wc_res[i]) >= MAX_ARGV_LEN)
		{
			print_shell_error(NULL, MAX_ARGV_LEN_ERR_MSG);
			return (COMMON_FAILURE);
		}
		op->argv[op->argc] = ft_strdup(wc_res[i]);
		if (!op->argv[op->argc])
			return (perror("malloc"), COMMON_SYS_ERR);
		++op->argc;
		++i;
	}
	return (COMMON_SUCCESS);
}

static int	expand_one_field(t_operand *op, t_vector *vec_pair[],
				size_t start, size_t end)
{
	int			fret;
	size_t		i;
	char		**wc_res;
	char		*exp_str;
	char		*qmask_str;
	t_vector	*field_pair[2];

	if (end <= start)
		return (COMMON_SUCCESS);
	if (exp_vectors_init(field_pair, end - start + 1) != COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	exp_str = (char *)vec_pair[EXP_RES]->data;
	qmask_str = (char *)vec_pair[QMASK]->data;
	i = start;
	while (i < end)
	{
		vector_push_back_char(field_pair[EXP_RES], exp_str[i]);
		vector_push_back_char(field_pair[QMASK], qmask_str[i]);
		++i;
	}
	vector_push_back_char(field_pair[EXP_RES], '\0');
	wc_collapse_conseq_asterisks(field_pair[EXP_RES], field_pair[QMASK]);
	if (wc_alloc_res(&wc_res) != COMMON_SUCCESS)
		return (exp_vectors_free(field_pair), COMMON_SYS_ERR);
	fret = expand_wildcards(wc_res,
			(char *)field_pair[EXP_RES]->data, field_pair[QMASK]);
	if (fret == COMMON_SUCCESS)
		fret = append_wc_matches(op, wc_res);
	wc_free_res(&wc_res);
	exp_vectors_free(field_pair);
	return (fret);
}

static int	append_split_fields(t_operand *op, t_vector *vec_pair[])
{
	int		fret;
	size_t	i;
	size_t	start;
	char	*exp_str;
	char	*qmask_str;

	exp_str = (char *)vec_pair[EXP_RES]->data;
	qmask_str = (char *)vec_pair[QMASK]->data;
	i = 0;
	while (exp_str[i] != '\0')
	{
		while (exp_str[i] == ' ' && qmask_str[i] == (char)IND_QNONE)
			++i;
		if (exp_str[i] == '\0')
			break ;
		start = i;
		while (exp_str[i] != '\0'
			&& !(exp_str[i] == ' ' && qmask_str[i] == (char)IND_QNONE))
			++i;
		fret = expand_one_field(op, vec_pair, start, i);
		if (fret != COMMON_SUCCESS)
			return (fret);
	}
	return (COMMON_SUCCESS);
}

void	exp_expand_argredir_loop(t_shell *msh, char *tok_str, t_vector *vec_pair[])
{
	size_t		i;
	size_t		slen;
	bool		f_extract;
	char		dlr_varname[MAX_ENV_VAL_LEN];
	t_ind_type	state;

	i = 0;
	slen = ft_strlen(tok_str);
	state = IND_QNONE;
	while (i < slen)
	{	
		if (tok_str[i] == '"')
			exp_process_double_quote(tok_str, &i, vec_pair, &state);
		else if (tok_str[i] == '\'')
			exp_process_single_quote(tok_str, &i, vec_pair, &state);
		else if (exp_tilde_found_argredir(tok_str, i, state))
			exp_expand_tilde(msh, vec_pair, state);
		else if (tok_str[i] == '$' && state != IND_QSINGLE)
		{
			f_extract = exp_extract_dlr_varname(dlr_varname, tok_str, &i);
			if (!f_extract)
			{
				vector_push_back_char(vec_pair[EXP_RES], '$');
				vector_push_back_char(vec_pair[QMASK], (char)state);
			}
			else
				exp_expand_variable(msh, vec_pair, dlr_varname, state);
		}
		else
		{
			vector_push_back_char(vec_pair[EXP_RES], tok_str[i]);
			vector_push_back_char(vec_pair[QMASK], (char)state);
		}
		++i;
	}
	vector_push_back_char(vec_pair[EXP_RES], '\0');
}
