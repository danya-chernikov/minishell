#ifndef PAR_H
# define PAR_H

typedef struct s_parser_data	t_parser_data;

# include "aux.h"

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

void	init_open_par(t_parser_data *d);
void	init_close_par(t_parser_data *d);
void	init_pars(t_pair *pars);
t_ll	get_par_by_prompt_ind(t_parser_data *d,
			size_t prompt_ind,
			t_par_type ptype);

#endif
