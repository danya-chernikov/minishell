#include "expansion.h"
#include "shell.h"
#include "operand.h"
#include "wildcards.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

int		exp_process_argredir(t_shell *msh, t_operand *op, t_op_token *op_tok, size_t *opt_i)
{
	int			fret;
	char		*tok_str;
	char		**wc_res;
	t_vector	*vec_pair[2];

	fret = COMMON_SUCCESS;

	tok_str = op_tok->cnt;

	if (!exp_vectors_init(vec_pair, ft_strlen(tok_str) + 1))
		return (COMMON_SYS_ERR);

	exp_expand_argredir_loop(msh, tok_str, vec_pair);

	// Now `exp_res` is a wildcard mask. Let's eliminate
	// all asterisks that go consequtively in the `exp_res`
	// updating corresponding indecies in `qmask`	
	wc_collapse_conseq_asterisks(vec_pair[EXP_RES], vec_pair[QMASK]);

	// Then we'll just pass `exp_res` as
	// wildcard mask into expand_wildcards()
	if (wc_alloc_res(&wc_res) != COMMON_SUCCESS)
	{
		exp_vectors_free(vec_pair);
		return (COMMON_SYS_ERR);
	}

	fret = expand_wildcards(wc_res,
		(char *)vec_pair[EXP_RES]->data, vec_pair[QMASK]);
	if (fret != COMMON_SUCCESS)
	{
		exp_vectors_free(vec_pair);
		return (fret);
	}

	// If previous token was a redirection
	// (it can be any redirection except heredoc)
	if (*opt_i > 0 && exp_token_is_redirect(&op->tokens[*opt_i - 1]))
	{
		// Heredocs delimiter: never treat as a file path
		if (exp_token_is_heredoc(&op->tokens[*opt_i - 1]))
		{
			wc_free_res(&wc_res);
			exp_vectors_free(vec_pair);
			return (COMMON_SUCCESS);
		}

		// Regular redirection path: need a valid redir_ind
		if (op_tok->redir_ind < 0 || (size_t)op_tok->redir_ind >= op->red_cnt)
		{
			wc_free_res(&wc_res);
			exp_vectors_free(vec_pair);
			return (COMMON_SUCCESS);
		}

		// It means we were handling a redirection operand/path
		// If globbing gave us more than one coincidence
		if (wc_res[1][0] != '\0')
		{
			print_shell_error(NULL, AMBIG_REDIRECT_ERR_MSG);
			wc_free_res(&wc_res);
			exp_vectors_free(vec_pair);
			return (COMMON_FAILURE);
		}
		// Update the corresponding redirection path (this case
		// we have the unique wildcards expansion result)
		free(op->redirs[op_tok->redir_ind].path);
		op->redirs[op_tok->redir_ind].path = ft_strdup(wc_res[0]);
		if (!op->redirs[op_tok->redir_ind].path)
		{
			perror("malloc");
			wc_free_res(&wc_res);
			exp_vectors_free(vec_pair);
			return (COMMON_SYS_ERR);
		}
		wc_free_res(&wc_res);
		exp_vectors_free(vec_pair);
	}
	else
	{
		// We were handling an argument
		size_t	arg_i;
			
		// Mark that all variables created while parsing
		// current operand were per-command variables.
		// It implies we'll not be creating any more
		// variables while parsing the current operand
		op->f_per_cmd = true;

		// Add all globbing results into arguments
		// (if there was no globbing performed
		// the expans_wildcards() will just put
		// into wc_res[0] its mask vec_pair[QMASK])
	
		arg_i = 0;
		while (arg_i < WC_MAX_FILES_NUM && wc_res[arg_i][0] != '\0')
		{
			if (arg_i >= MAX_ARGC_NUM - 1)
			{
				print_shell_error(NULL, MAX_ARGC_NUM_ERR_MSG);
				return (COMMON_FAILURE);
			}
			if (ft_strlen(wc_res[arg_i]) >= MAX_ARGV_LEN - 1)
			{
				print_shell_error(NULL, MAX_ARGV_LEN_ERR_MSG);
				return (COMMON_FAILURE);
			}
			op->argv[op->argc] = ft_strdup(wc_res[arg_i]);
			if (!op->argv[op->argc])
			{
				perror("malloc");
				fret = COMMON_SYS_ERR;
				break ;
			}
			++op->argc;
			++arg_i;
		}
	}

	wc_free_res(&wc_res);
	exp_vectors_free(vec_pair);
	return (fret);
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
			{
				exp_expand_variable(msh, vec_pair, dlr_varname, state);
			}
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
