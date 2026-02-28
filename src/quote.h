/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:52:13 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 22:32:27 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H

# include <stdbool.h>
# include <stddef.h>

# define MAX_QUOTES_NUM	256
# define QSINGLE		0
# define QDOUBLE		1

typedef struct s_parser_data	t_parser_data;
typedef struct s_operand		t_operand;

typedef enum e_quote_type
{
	DOUBLE_QUOTE,
	SINGLE_QUOTE
}	t_quote_type;

/* li - left-side index;
 * ri - right-side index */
typedef struct s_quote_interval
{
	size_t			li;
	size_t			ri;
	t_quote_type	type;

}	t_quote_int;

/* quote.c */
bool	quotes_parser(char *str, t_quote_int *quotes, size_t *qpair_cnt);
int		remove_syntax_quotes(char *str, t_quote_int *quotes, size_t qpair_cnt);

/* quote2.c */
bool	is_inside_quotes_uni(t_quote_int *quotes, size_t qpair_cnt, size_t ind);
bool	is_inside_op_quotes(t_operand *op, size_t op_i);
bool	is_inside_op_quotes_single(t_operand *op, size_t op_i);
bool	is_inside_op_quotes_double(t_operand *op, size_t op_i);
bool	is_syntax_quote(t_quote_int *quotes, size_t qpair_cnt, size_t ind);

#endif
