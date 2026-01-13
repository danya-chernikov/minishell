#ifndef PARSER_H
# define PARSER_H

/*# include <stdint.h>
# include <sys/types.h>
# include <sys/stat.h>*/

#include <stdlib.h>

# include "par.h"
# include "quote.h"
# include "token.h"
# include "operand.h"
# include "aux.h"
# include "error.h"

# define MAX_FORMAT_STR_LEN	64
# define PROMPT_INV_LEN		64	// Maximum length of user's prompt invitation string
# define MAX_PIPES_NUM		128
# define MAX_PROGRAM_PATH	256
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
	size_t		qpair_cnt;					// Counter off quote pairs

}	t_parser_data;

int		parser_init(t_parser_data *d, char *rline_buf);
bool	parser_engine(t_parser_data *d);
void	handle_open_par(t_parser_data *d, int opar_ind, bool *f_noerr);
void	handle_close_par(t_parser_data *d, bool *f_noerr);

#endif
