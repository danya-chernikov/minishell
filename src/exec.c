#include "exec.h"
#include "shell.h"
#include "operand.h"
#include "token.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>

int	exec_ops(t_shell *msh, int *ret_code)
{
	(void)msh;
	(void)ret_code;

	int	fret = 1;

	/*int				ti;
	int				fret;
	t_parser_data	*pd;
	t_token			*token;

	ti = 0;
	pd = msh->pd;
	fret = COMMON_SUCCESS;
	while (ti < pd->token_cnt)
	{
		token = pd->tokens[ti];
		if (token.type == OPERAND)
		{

			fret = expand_wildcards(token->op);
			if (fret != COMMON_SUCCESS)
				return (fret);


		}
		++ti;
	}*/

	return (fret);
}

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

int	close_pipes(t_shell *msh)
{
	size_t	i;

	i = 0;
	while (i < msh->pd->pipe_cnt)
	{
		if (close(msh->pd->pipes[i][READ_END]) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
		if (close(msh->pd->pipes[i][WRITE_END]) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
		++i;
	}
	return (COMMON_SUCCESS);
}
