#include "redirect.h"
#include "operand.h"
#include "libft.h"

bool	is_redir_in(t_operand *op, size_t op_i)
{
	if (op->name[op_i] == '<' && !is_inside_op_quotes(op, op_i))
		return (true);
	return (false);
}

bool	is_redir_out(t_operand *op, size_t op_i)
{
	if (op->name[op_i] == '>' && !is_inside_op_quotes(op, op_i))
		return (true);
	return (false);
}

bool	is_redir_app(t_operand *op, size_t op_i)
{
	if (op->name[op_i] == '>' && !is_inside_op_quotes(op, op_i) &&
		op_i < ft_strlen(op->name) - 1 &&
		op->name[op_i + 1] == '>' && !is_inside_op_quotes(op, op_i + 1))
	{
		return (true);
	}
	return (false);
}

bool	is_heredoc(t_operand *op, size_t op_i)
{
	if (op->name[op_i] == '<' && !is_inside_op_quotes(op, op_i) &&
		op_i < ft_strlen(op->name) - 1 &&
		op->name[op_i + 1] == '<' && !is_inside_op_quotes(op, op_i + 1))
	{
		return (true);
	}
	return (false);
}
