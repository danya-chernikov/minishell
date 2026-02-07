#include "wildcards.h"
#include "operand.h"
#include "error.h"

/* Let's extract the substring where we found the asterisk.
 * Determine its left and right boundary indices.
 *
 * Then split the operand string into three chunks, where
 * the second chunk is the extracted substring containing
 * the asterisk(s).
 *
 * Next collapse consecutive asterisks in the middle chunk.
 * This middle chunk becomes the mask.
 *
 * Then pass this mask to expand_cur_dir_wildcards(). It
 * returns an array of char *, where each element is a
 * filename matching the mask.
 *
 * Repeat until there are no asterisks outside of quotes
 * (single or double) */
int	expand_wildcards(t_operand *op)
{
	(void)op;
	/*
	size_t	op_i;
	size_t	slen;

	op_i = 0;
	slen = ft_strlen(op->name);
	while (op_i < slen)
	{
		if (op->name[op_i] == '*' && is_inside_op_quotes(op, op_i))
		{
		}
		++op_i;
	}*/
	return (COMMON_SUCCESS);
}
