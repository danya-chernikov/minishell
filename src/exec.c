#include "exec.h"
#include "shell.h"
#include "prompt_parser.h"
#include "operand.h"
#include "token.h"
#include "expansion.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int	exec_ops(t_shell *msh, int *ret_code)
{
	int				fret;
	size_t			ti; // Token index
	t_parser_data	*pd;
	
	(void)ret_code;
	ti = 0;
	pd = msh->pd;
	fret = COMMON_SUCCESS;
	// Go through all token-operands
	while (ti < pd->token_cnt)
	{
		t_token		*token;

		token = &pd->tokens[ti];
		if (token->type == OPERAND)
		{
			{
				int			fret;
				size_t		opt_i; // Operand's token index
				t_operand	*op;
				t_op_token	*op_tok;

				op = token->op;

				fret = exp_divide_op_str_on_tokens(op);
				if (fret != COMMON_SUCCESS)
					return (fret);

				fret = exp_update_op_tokens_quote_intervals(op);
				if (fret != COMMON_SUCCESS)
					return (fret);

				opt_i = 0;
				while (opt_i < op->token_cnt)
				{
					op_tok = &op->tokens[opt_i];

					// >, >>, <, <<
					if (exp_token_is_redirect(op_tok))
					{
						++opt_i; // Just skip this operand token
						continue ;
					}
					else if (exp_token_is_assignment(op, op_tok)) // Assignment
					{
						fret = exp_process_assignment(msh, op, op_tok, &opt_i);
						if (fret == CONTINUE)
							continue ;
						else if (fret != COMMON_SUCCESS)
							return (fret);
					}

					// If we're here the current operand's token
					// is a regular argument or a redirection opernad

					// If the previous token was << (means current
					// token is a heredoc delimiter)
					if (exp_token_is_heredoc(&op->tokens[opt_i - 1]))
					{
						// Skip it (cause we've already processed
						// it on the preliminary parsing stage)
						++opt_i;
						continue ;
					}

					fret = exp_process_argredir(msh, op_tok);
					if (fret != COMMON_SUCCESS)
						return (fret);

					++opt_i;
				} // while (opt_i < op->token_cnt)
			
			}

		} // if (token->type == OPERAND)
	
		++ti;

	} // while (ti < pd->token_cnt)

	return (fret);
}
