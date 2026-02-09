#include "exec.h"
#include "operand.h"

#include "vector.h"
#include "libft.h"
#include "error.h"

/* First we initialize `exp_res` and then `qmask` */
int	exec_vectors_init(t_vector *vec_pair[], size_t cap)
{
	if (!vector_init(vec_pair[EXEC_EXP_RES], CHAR, cap))
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	if (!vector_init(vec_pair[EXEC_QMASK], CHAR, cap))
	{
		perror("malloc");
		vector_free(vec_pair[EXEC_EXP_RES]);
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* If state == IND_QNONE for index `i` then it's obvious
 * that tok_str[i + 1] will be unquoted or be a quote */
bool	exec_tilde_found(char *tstr, size_t i, size_t eqsign_ind, t_ind_type state)
{
	if (tstr[i] == '~' &&
		i - 1 == eqsign_ind &&
		state == IND_QNONE &&
		(tstr[i + 1] == '/' || tstr[i + 1] == '\0'))
	{
		return (true);
	}
	return (false);
}

bool	exec_token_is_assignment(t_operand *op, t_op_token *op_tok)
{
	size_t	i;
	size_t	slen;

	i = 0;
	slen = ft_strlen(op_tok->cnt);
	while (i < slen)
	{
		// If we found = outside any quotes
		if (op_tok->cnt[i] == '=' &&
			!is_inside_quotes_uni(op_tok->quotes, op_tok->qpair_cnt, i))
		{
			if (i > 0) // On the left from '=' there are some symbols
				if (op->argc == 0) // We have not found any argument yet
					return (true);
		}
		++i;
	}
	return (false);
}
