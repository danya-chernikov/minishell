#ifndef AUX_H
# define AUX_H

# include <stdbool.h>
# include <stdint.h>

# include "libft.h"

typedef unsigned char		t_uchar;
typedef unsigned short		t_ushort;
typedef unsigned int		t_uint;
typedef unsigned long		t_ul;
typedef long long			t_ll;
typedef unsigned long long	t_ull;
typedef long double			t_ld;

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
