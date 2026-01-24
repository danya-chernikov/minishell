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

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		ops[i].name = (char *)ft_calloc(MAX_OP_LEN, sizeof (char));
		if (!ops[i].name)
		{
			--i;
			while (i >= 0)
				free(ops[i--].name);
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		ops[i].write_end = DEFAULT_FD;
		ops[i].read_end = DEFAULT_FD;
		++i;
	}
	return (COMMON_SUCCESS);
}

void	ops_free(t_operand *ops)
{
	if (ops->name)
		free(ops->name);
}
