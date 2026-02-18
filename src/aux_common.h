#ifndef AUX_COMMON_H
# define AUX_COMMON_H

# include <stdbool.h>

# define BREAK		5
# define CONTINUE	6

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
bool	is_variable_name_correct(char *var_name);
bool	is_varname_symbol_permitted(char symbol);

/* aux_common3.c */
int		perror_and_return(char *where, int ret_code);
int		free_one_and_return(void *ptr, int ret_code);
int		free_two_and_return(void *ptr1, void *ptr2, int ret_code);
bool	is_surrounded_quotes(char *str); // Obsolete
void	erase_quotes_no_check(char *str); // Obsolete

#endif
