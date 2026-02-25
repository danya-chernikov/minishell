#include "operand.h"
#include "env.h"

#include "libft.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static void	free_tokens_loop(t_operand *ops);
static void	free_redirs_loop(t_operand *ops);

/* Assign the default value to
 * the pipes of all operators */
int	ops_init(t_operand *ops)
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
		ops[i].my_env = NULL;
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

/* Allocates memory for tokens array
 * of the operand `op` */
int	op_token_init(t_operand *op)
{
	size_t	i;

	op->tokens = malloc(MAX_OP_TOKENS_NUM * sizeof (t_op_token));
	if (!op->tokens)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	i = 0;
	while (i < MAX_OP_TOKENS_NUM)
	{
		op->tokens[i].cnt = NULL;
		op->tokens[i].qpair_cnt = 0;
		++i;
	}
	return (COMMON_SUCCESS);
}

int	op_env_init(t_operand *op)
{
	int	fres;

	op->my_env = malloc(1 * sizeof (t_env));
	if (!op->my_env)
		return (COMMON_SYS_ERR);
	fres = env_init(op->my_env, NULL);
	if (fres != COMMON_SUCCESS)
	{
		free(op->my_env);
		op->my_env = NULL;
		return (fres);
	}
	return (COMMON_SUCCESS);
}

void	ops_free(t_operand *ops)
{
	size_t	i;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		if (ops[i].name)
		{
			free(ops[i].name);
			ops[i].name = NULL;
		}
		if (ops[i].my_env)
		{
			env_free(ops[i].my_env);
			free(ops[i].my_env);
			ops[i].my_env = NULL;
		}
		if (ops[i].tokens)
			free_tokens_loop(ops);
		free_redirs_loop(ops);
		ops[i].red_cnt = 0;
		ops[i].token_cnt = 0;
		ops[i].argc = 0;
		++i;
	}
}

static void	free_tokens_loop(t_operand *ops)
{
	int	j;

	j = 0;
	while (j < MAX_OP_TOKENS_NUM)
	{
		if (ops[i].tokens[j].cnt)
		{
			free(ops[i].tokens[j].cnt);
			ops[i].tokens[j].cnt = NULL;
		}
		++j;
	}
	free(ops[i].tokens);
	ops[i].tokens = NULL;
}

static void	free_redirs_loop(t_operand *ops)
{
	int	j;

	j = 0;
	while (j < MAX_REDIRS_NUM)
	{
		if (ops[i].redirs[j].path)
		{
			free(ops[i].redirs[j].path);
			ops[i].redirs[j].path = NULL;
		}
		if (ops[i].redirs[j].hd.content)
		{
			free(ops[i].redirs[j].hd.content);
			ops[i].redirs[j].hd.content = NULL;
		}
		if (ops[i].redirs[j].hd.delim)
		{
			free(ops[i].redirs[j].hd.delim);
			ops[i].redirs[j].hd.delim = NULL;
		}
		++j;
	}
}
