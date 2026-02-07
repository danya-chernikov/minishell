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
	(void)ret_code;

	int				fret;
	size_t			ti;

	t_parser_data	*pd;
	
	ti = 0;
	pd = msh->pd;
	fret = COMMON_SUCCESS;
	// Go through all token-operands
	while (ti < pd->token_cnt)
	{
		t_token	*token;

		token = &pd->tokens[ti];
		if (token->type == OPERAND)
		{
			{
				size_t		op_i;
				size_t		ti;
				size_t		opstr_len;
				char		*op_str;
				char		*tcnt; // Operand's token content
				t_operand	*op;

				// Divide operand's string 'name' on tokens by spaces
				// (which are located outside any quote intervals of course)
				// Each operand contains at least one token
				op = token->op;
				op_str = op->name;
				opstr_len = ft_strlen(op_str);

				tcnt = (char *)malloc(MAX_OP_TOKEN_LEN * sizeof(char));
				if (!tcnt)
				{
					perror("malloc");
					return (COMMON_SYS_ERR);
				}
				ti = 0;
				op_i = 0;
				while (op_i < opstr_len)
				{
					if (op_i > MAX_OP_TOKEN_LEN - 1) // Overflow check
					{
						print_shell_error(NULL, MAX_OP_TOK_LEN_ERR_MSG);
						return (COMMON_FAILURE);
					}
					if (op_str[op_i] == ' ' && !is_inside_op_quotes(op, op_i))
					{
						if (op->token_cnt > MAX_OP_TOKENS_NUM - 1) // Overflow check
						{
							print_shell_error(NULL, MAX_OP_TOK_NUM_ERR_MSG);
							return (COMMON_FAILURE);
						}
						tcnt[ti] = '\0';
						ti = 0;
						op->tokens[op->token_cnt].cnt = tcnt;
						++op->token_cnt;
						tcnt = (char *)malloc(MAX_OP_TOKEN_LEN * sizeof(char));
						if (!tcnt)
						{
							perror("malloc");
							return (COMMON_SYS_ERR);
						}
						skip_spaces(op_str, &op_i);
						continue ;
					}
					tcnt[ti] = op_str[op_i];
					++op_i;
					++ti;
				}
				tcnt[ti] = '\0';
				op->tokens[op->token_cnt].cnt = tcnt;
				++op->token_cnt;
			}

			// Update quote intervals for each operand's token
			{
				int			fret;
				size_t		ti;
				t_operand	*op;
				t_op_token	*op_token;

				op = token->op;
				ti = 0;
				while (ti < op->token_cnt)
				{
					op_token = &op->tokens[ti];
					fret = quotes_parser(op_token->cnt, op_token->quotes, &op_token->qpair_cnt);
					if (fret != COMMON_SUCCESS)
						return (fret);
					++ti;
				}
			}

#if DEBUG == 1
		dbg_print_operand_tokens(pd);
#endif

		} // if (token->type == OPERAND)
	
		++ti;

	} // while (ti < pd->token_cnt)

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
