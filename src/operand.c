#include "operand.h"
#include "env.h"
#include "libft.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* Assign the default value to
 * the pipes of all operators */
int		ops_init(t_operand *ops)
{
	int	i;
	int	j;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		ops[i].name = NULL;
		ops[i].write_end = DEFAULT_FD;
		ops[i].read_end = DEFAULT_FD;
		ops[i].tokens = NULL;
		env_init(ops[i].vars);
		ops[i].f_per_cmd = false;
		j = 0;
		while (j < MAX_REDIRS_NUM)
		{
			ops[i].redirs[j].path = NULL;
			ops[i].redirs[j].hd.content = NULL;
			ops[i].redirs[j].hd.delim = NULL;
			++j;
		}
		++i;
	}
	return (COMMON_SUCCESS);
}

int	op_token_init(t_operand *op)
{
	size_t	i;

	op->tokens = malloc(MAX_OP_TOKENS_NUM * sizeof *op->tokens);
	if (!op->tokens)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	i = 0;
	while (i < MAX_OP_TOKENS_NUM)
	{
		op->tokens[i].cnt = NULL;
		++i;
	}
	return (COMMON_SUCCESS);
}

void	ops_free(t_operand *ops)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		if (ops[i].name)
			free(ops[i].name);
		env_free(ops[i].vars);
		// Free tokens
		j = 0;	
		while (j < MAX_OP_TOKENS_NUM)
		{
			if (&ops[i].tokens[j])
			{
				if (&ops[i].tokens[j].cnt)
					free(&ops[i].tokens[j].cnt);
				free(&ops[i].tokens[j]);
			}
			++j;
		}
		// Free redirections
		j = 0;
		while (j < MAX_REDIRS_NUM)
		{
			if (ops[i].redirs[j].path)
				free(ops[i].redirs[j].path);
			if (ops[i].redirs[j].hd.content)
				free(ops[i].redirs[j].hd.content);
			if (ops[i].redirs[j].hd.delim)
				free(ops[i].redirs[j].hd.delim);
			++j;
		}
		++i;
	}
}
