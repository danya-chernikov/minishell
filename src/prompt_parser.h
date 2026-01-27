#ifndef PROMPT_PARSER_H
# define PROMPT_PARSER_H

#include <stdlib.h>

# include "par.h"
# include "quote.h"
# include "token.h"
# include "operand.h"
# include "aux_common.h"
# include "error.h"

# define MAX_FORMAT_STR_LEN	256
# define MAX_PIPES_NUM		256
# define MAX_PROGRAM_PATH	4096
# define NONE_INDEX			-1
# define NONE_PIPE			-1

typedef struct s_parser_data
{
	char		*prompt;					// Prompt entered by user
	size_t		pi;							// Prompt index

	size_t		pipe_cnt;					// Pipe counter
	int			pipes[MAX_PIPES_NUM][2];	// All pipes array
	
	size_t		op_cnt;						// Operand counter
	t_operand	ops[MAX_OPS_NUM];			// Operands (programs to launch)
	
	size_t		opar_num;					// Number of all opening-parentheses
	size_t		all_open_pars[MAX_PAR_NUM][2]; // Indexes of all opening-parentheses
	
	t_ll		opar_cnt;					// Opening-parentheses counter (must be int)
	size_t		open_par[MAX_PAR_NUM];		// Opening-parentheses indexes found and thier flags
	
	t_ll		cpar_cnt;					// Closing-parentheses counter (for now let it be int)
	size_t		close_par[MAX_PAR_NUM][2];	// Closing-parentheses indexes found and their flags
	
	t_pair		pars[MAX_PAR_NUM];			// A member that represents each parentheses pair
	size_t		par_cnt;					// Parentheses pair counter
	
	size_t		token_cnt;					// Token counter
	t_token		tokens[MAX_TOKENS_NUM];		// Here we store all tokens we found during parsing
	
	t_quote_int quotes[MAX_QUOTES_NUM];		// All quotes pairs found in the prompt
	size_t		qpair_cnt;					// Counter of quote pairs

}	t_parser_data;

/* prompt_parser.c */
int		parser_init(t_parser_data *d, char *rline_buf);
void	parser_free(t_parser_data *d);

/* prompt_parser2.c */
int		parser_engine(t_parser_data *d);
int		handle_open_par(t_parser_data *d, int opar_ind);
int		handle_close_par(t_parser_data *d);

/* prompt_parser3.c */
void	token_push(t_parser_data *d, t_token_type type);
int		operand_push(t_parser_data *d, size_t pi);
bool	check_empty_par(t_parser_data *d);
int		later_goes_open_par(char *str, size_t ind);

/* prompt_parser4.c */
bool	its_logical_AND(char *prompt, size_t plen, size_t pi);
bool	its_logical_OR(char *prompt, size_t plen, size_t pi);
bool	its_PIPE(char *prompt, size_t plen, size_t pi);

/* prompt_parser5.c */
void	skip_spaces(char *prompt, size_t *pi);
bool	is_special_char(char sym);
bool	is_inside_quotes(t_parser_data *d, size_t pi);
bool	is_special_char_outside_quotes(t_parser_data *d, size_t pi);
void	remove_right_spaces(char *prompt);
void	remove_left_spaces(char *prompt);

#endif
