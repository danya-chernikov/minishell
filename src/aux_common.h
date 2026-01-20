#ifndef AUX_COMMON_H
# define AUX_COMMON_H

# include <stdbool.h>

bool	strings_equal(char *str1, char *str2);
void	remove_newline(char *str);
char	*get_full_path(char *rel_path);
char	*expand_homedir(char *str, char *home);
int		div2_str_by_delim(char *str, char delim, char **part1, char **part2);
int		div2_str_by_delim_alloc(char **part1, char **part2);

#endif
