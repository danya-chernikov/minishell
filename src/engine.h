#ifndef ENGINE_H
# define ENGINE_H

# include "prompt_parser.h"

int	shell_engine(char *prompt, int *ret_code);
int	comments_parser(t_parser_data *d);
int	operands_quotes_parser(t_parser_data *d);

int	redirections_parser(t_parser_data *d);
int	add_redir_in(t_operand *op, size_t *op_i);
int	add_redir_out(t_operand *op, size_t *op_i);
int	add_redir_app(t_operand *op, size_t *op_i);
int	add_heredoc(t_operand *op, size_t *op_i);

#endif
