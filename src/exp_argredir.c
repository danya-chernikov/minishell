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
	t_vector	*vec_pair[2];

	if (exp_vectors_init(vec_pair, ft_strlen(op_tok->cnt) + 1)
		!= COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	exp_expand_argredir_loop(msh, op_tok->cnt, vec_pair);
	wc_collapse_conseq_asterisks(vec_pair[EXP_RES], vec_pair[QMASK]);
	fret = exp_apply_token_result(op, op_tok, *opt_i, vec_pair);
	exp_vectors_free(vec_pair);
	return (fret);
}

int	exp_apply_token_result(t_operand *op, t_op_token *op_tok,
		size_t opt_i, t_vector *vec_pair[])
{
	if (opt_i > 0 && exp_token_is_redirect(&op->tokens[opt_i - 1]))
		return (exp_apply_redir_result(op, op_tok,
				&op->tokens[opt_i - 1], vec_pair));
	return (append_split_fields(op, vec_pair));
}

int	exp_apply_redir_result(t_operand *op, t_op_token *op_tok,
		t_op_token *prev_tok, t_vector *vec_pair[])
{
	int		fret;
	char	**wc_res;

	if (exp_token_is_heredoc(prev_tok))
		return (COMMON_SUCCESS);
	if (wc_alloc_res(&wc_res) != COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	fret = expand_wildcards(wc_res,
			(char *)vec_pair[EXP_RES]->data, vec_pair[QMASK]);
	if (fret == COMMON_SUCCESS)
		fret = update_redir_path(op, op_tok, wc_res);
	wc_free_res(&wc_res);
	return (fret);
}

int	update_redir_path(t_operand *op, t_op_token *op_tok, char **wc_res)
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
		return (perror("malloc"), COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

int	append_wc_matches(t_operand *op, char **wc_res)
{
	size_t	i;

	i = 0;
	op->f_per_cmd = true;
	while (i < WC_MAX_FILES_NUM && wc_res[i][0] != '\0')
	{
		if (op->argc >= MAX_ARGC_NUM - 1)
			return (print_shell_error(NULL, MAX_ARGC_NUM_ERR_MSG),
				COMMON_FAILURE);
		if (ft_strlen(wc_res[i]) >= MAX_ARGV_LEN)
			return (print_shell_error(NULL, MAX_ARGV_LEN_ERR_MSG),
				COMMON_FAILURE);
		op->argv[op->argc] = ft_strdup(wc_res[i]);
		if (!op->argv[op->argc])
			return (perror("malloc"), COMMON_SYS_ERR);
		++op->argc;
		++i;
	}
	return (COMMON_SUCCESS);
}
