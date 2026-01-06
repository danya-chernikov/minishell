#ifndef AUX_H
# define AUX_H

# include <stdbool.h>
# include <stdint.h>

# include "libft.h"

typedef long long		t_ll;
typedef unsigned char	u_char;

/* The value of -1 means the
 * index was not assigned */
typedef struct s_pair
{
	t_ll	first;
	t_ll	second;
}	t_pair;

void	remove_right_spaces(char *prompt);
bool	check_empty_par(char *prompt);
int		later_goes_open_par(char *str, size_t ind);
void	skip_spaces(char *prompt, size_t *pi);

#endif
