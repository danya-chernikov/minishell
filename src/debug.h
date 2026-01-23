#ifndef DEBUG_H
# define DEBUG_H

# define SUBSHELLS_PIDS_LOG	"subshells_PIDs"
# define BUF_SIZE			1024

# include <stdio.h>

# include "prompt_parser.h"
# include "config.h"
# include "history.h"

void	dbg_print_configs(t_configs *cnf, t_conf_type ctype);
void	dbg_print_filehistory(t_history *history);
void	dbg_prompt_parser_print_all(t_parser_data *d);
void	dbg_prompt_parser_print_quotes(t_parser_data *d);
void	dbg_prompt_parser_print_parsed_data(t_parser_data *d);
void	dbg_prompt_parser_print_tokens(t_parser_data *d);
void	dbg_prompt_parser_print_parentheses(t_parser_data *d);

#endif
