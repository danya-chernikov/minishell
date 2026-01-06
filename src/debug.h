#ifndef DEBUG_H
# define DEBUG_H

# define SUBSHELLS_PIDS_LOG	"subshells_PIDs"
# define BUF_SIZE			1024

# include <stdio.h>

# include "parser.h"

void	print_quotes(t_parser_data *d);
void	print_parsed_data(t_parser_data *d);
void	print_tokens(t_parser_data *d);
void	print_parentheses(t_parser_data *d);

#endif
