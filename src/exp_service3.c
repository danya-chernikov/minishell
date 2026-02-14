#include "expansion.h"
#include "operand.h"
#include "prompt_parser.h"

#include <stdio.h>
#include <stdlib.h>

int		exp_alloc_argv(t_operand *op)
{
	op->argv = (char **)malloc(MAX_ARGC_NUM * sizeof (char *));
	if (!op->argv)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

void	exp_free_argv(t_parser_data *d)
{
	size_t		op_i;
	int			arg_i;
	t_operand	*op;

	op_i = 0;
	while (op_i < d->op_cnt)
	{
		op = &d->ops[op_i];
		arg_i = 0;
		while (arg_i < op->argc)
		{
			free(op->argv[arg_i]);
			++arg_i;
		}
		++op_i;
	}
}
