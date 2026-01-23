#ifndef PAR_H
# define PAR_H

# include "aux_common.h"

typedef struct s_parser_data	t_parser_data;

# define NONE_PAR_IND	-1
# define NOT_CLOSED_PAR	0	// This parenthesis wasn't closed yet (We didn't pass it)
# define CLOSED_PAR		1	// Thie parenthesis was already closed
# define MAX_PAR_NUM	128	// Maximum parentheses number

/* Parenthesis type */
typedef enum e_par_type
{
	OPENING_PAR,
	CLOSING_PAR
}	t_par_type;

void	pars_init(t_pair *pars);
void	open_par_init(t_parser_data *d);
void	close_par_init(t_parser_data *d);
t_ll	get_par_by_prompt_ind(t_parser_data *d, size_t pind, t_par_type ptype);

#endif
