#ifndef AUX_COMMON_H
# define AUX_COMMON_H

# include <stdbool.h>

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

/* aux_common.c */
bool	strings_equal(char *str1, char *str2);
void	remove_newline(char *str);
char	*get_full_path(char *rel_path);
int		div2_str_by_delim(char *str, char delim, char **part1, char **part2);
int		div2_str_by_delim_alloc(char **part1, char **part2);

/* aux_common2.c */
char	*expand_homedir(char *str, char *home);
bool	contains_quote(char *str);
bool	is_surrounded_quotes(char *str); // Obsolete
void	erase_quotes_no_check(char *str); // Obsolete

#endif
