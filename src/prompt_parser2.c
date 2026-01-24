#include "prompt_parser.h"

void	token_push(t_parser_data *d, t_token_type type)
{
	d->tokens[d->token_cnt].type = type;
	if (type == OPERAND)
		d->tokens[d->token_cnt].op = (t_operand *)&d->ops[d->op_cnt - 1];
	d->tokens[d->token_cnt].start_pi = d->pi;
	++d->token_cnt;
}

/* Traverse all symbols until the next operand or
 * parenthesis (that is located outside of any
 * quotes interval of course), and add them to the
 * operands array. All this symbols will represent
 * a new operand (program built-in name/path plus
 * its arguments or variables assignments). Also
 * it checks for potential buffer overflows.
 *
 *     plen	- promopt length;
 *     pi	- prompt index */

int		operand_push(t_parser_data *d, size_t plen)
{
	size_t	i;

	i = 0;
	while (d->pi < plen && !is_special_char_outside_quotes(d, d->pi))
	{
		if (i > MAX_OP_LEN - 1)
		{
			print_prompt_parser_ext_error(TOO_LONG_OP_ERR_MSG);
			return (COMMON_FAILURE);
		}
		if (d->op_cnt > MAX_OPS_NUM - 1)
		{
			print_prompt_parser_ext_error(TOO_MANY_OPS_ERR_MSG);
			return (COMMON_FAILURE);
		}
		d->ops[d->op_cnt].name[i] = d->prompt[d->pi];
		++d->pi;
		++i;
	}
	d->ops[d->op_cnt].name[i] = '\0';
	++d->op_cnt;
	return (COMMON_SUCCESS);
}

/* Checks for existance of empty parentheses
 * outside any quotes intervals.
 * Sequences like: (), (( )), (((  ))), and etc.
 * Returns true if there are no empty sequences*/
bool	check_empty_par(t_parser_data *d)
{
	size_t	pi;
	size_t	plen;

	pi = 0;
	plen = ft_strlen(d->prompt);
	while (pi < plen)
	{
		if (d->prompt[pi] == '(' && !is_inside_quotes(d, pi))
		{
			++pi;
			skip_spaces(d->prompt, &pi);
			// Parsing error
			if (pi == plen ||
				(d->prompt[pi] == ')' && !is_inside_quotes(d, pi)))
			{
				print_prompt_parser_ext_error(EMPTY_PARS_ERR_MSG);
				return false;
			}
		}
		++pi;
	}
	return true;
}

/* Checks whether there is an opening parenthesis
 * later to the right, starting from index ind,
 * ignoring only spaces. If it finds '(' while
 * moving to the right and skipping all spaces,
 * it returns its index. Otherwise, it returns -1.
 * If it encounters any symbol other than a space
 * before finding '(', it also returns -1 */
int	later_goes_open_par(char *str, size_t ind)
{
	size_t	slen;

	++ind;
	slen = ft_strlen(str);
	while (ind < slen)
	{
		if (str[ind] == '(')
			return ind;
		if (str[ind] == ' ')
			++ind;
		else
			return -1;
	}
	return (-1);
}
