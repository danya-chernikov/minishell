#include "operand.h"

/* Assign the default value to
 * the pipes of all operators */
void	init_ops(t_operand *ops)
{
	size_t	i;

	i = 0;
	while (i < MAX_OPS_NUM)
	{
		ops[i].write_end = DEFAULT_FD;
		ops[i].read_end = DEFAULT_FD;
		++i;
	}
}
