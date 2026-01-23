#include "prompt_parser.h"

bool	its_logical_AND(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '&' && prompt[pi + 1] == '&')
		return (true);
	return (false);
}

bool	its_logical_OR(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '|' && prompt[pi + 1] == '|')
		return (true);
	return (false);
}

bool	its_PIPE(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '|' && prompt[pi + 1] != '|')
		return (true);
	return (false);	
}

void	token_push(t_parser_data *d, t_token_type type)
{
	d->tokens[d->token_cnt].type = type;
	if (type == OPERAND)
		d->tokens[d->token_cnt].op = (t_operand *)&d->ops[d->op_cnt];
	d->tokens[d->token_cnt].start_pi = d->pi;
	++d->token_cnt;
}
