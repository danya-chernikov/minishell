#include "prompt_parser.h"

#include <stdio.h>
#include <stdlib.h>

void	token_push(t_parser_data *d, t_token_type type)
{
	t_operand	*op;

	op = NULL;
	d->tokens[d->token_cnt].type = type;
	d->tokens[d->token_cnt].start_pi = d->pi;
	if (type == OPERAND)
	{
		op = (t_operand *)&d->ops[d->op_cnt - 1];
		d->tokens[d->token_cnt].op = op;
		d->tokens[d->token_cnt].start_pi = d->pi - ft_strlen(op->name);
	}
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
	int		fret;
	size_t	opname_len;
	char	opname[MAX_OPS_NUM];

	i = 0;
	while (d->pi < plen && !is_special_char_outside_quotes(d, d->pi))
	{
		if (i > MAX_OP_LEN - 1)
		{
			print_shell_error(NULL, TOO_LONG_OP_ERR_MSG);
			return (COMMON_FAILURE);
		}
		if (d->op_cnt > MAX_OPS_NUM - 1)
		{
			print_shell_error(NULL, TOO_MANY_OPS_ERR_MSG);
			return (COMMON_FAILURE);
		}
		opname[i] = d->prompt[d->pi];
		++d->pi;
		++i;
	}
	opname[i] = '\0';
	remove_right_spaces(opname); // DUCT TAPE! ( but works :3 )
	opname_len = ft_strlen(opname);
	d->ops[d->op_cnt].name = (char *)malloc((opname_len + 1) * sizeof (char));
	if (!d->ops[d->op_cnt].name)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	ft_strlcpy(d->ops[d->op_cnt].name, opname, opname_len + 1);
	// Initialize tokens of this operand
	fret = op_token_init(&d->ops[d->op_cnt]);
	if (fret != COMMON_SUCCESS)
		return (fret);
	// Initialize local environment of this operand
	fret = op_env_init(&d->ops[d->op_cnt]);
	if (fret != COMMON_SUCCESS)
		return (fret);

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
				print_shell_error(NULL, EMPTY_PARS_ERR_MSG);
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
