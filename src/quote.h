#ifndef QUOTE_H
# define QUOTE_H

# include <stdbool.h>
# include <stddef.h>

# define MAX_QUOTES_NUM	256

typedef struct s_parser_data	t_parser_data;
typedef struct s_operand		t_operand;

typedef enum e_quote_type
{
	DOUBLE_QUOTE,
	SINGLE_QUOTE

}	t_quote_type;

typedef struct s_quote_interval
{
	size_t			li;		// Left-side index
	size_t			ri;		// Right-side index
	t_quote_type	type;

}	t_quote_int;

bool	quotes_parser(t_parser_data *d);
bool	operand_quotes_parser(t_operand *op);
bool	is_inside_op_quotes(t_operand *op, size_t op_i);

#endif
