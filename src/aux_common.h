#ifndef AUX_COMMON_H
# define AUX_COMMON_H

# include <stdbool.h>

bool	strings_equal(char *str1, char *str2);
int		div2_str_by_delim(char *str, char delim, char **part1, char **part2);
int		div2_str_by_delim_alloc(char **part1, char **part2);

#endif
