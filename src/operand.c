#include "operand.h"
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

void	ops_free(t_operand *ops)
{
	int	i;
	int	j;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		if (ops[i].name)
			free(ops[i].name);
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
