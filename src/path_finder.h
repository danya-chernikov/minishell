#ifndef PATH_FINDER_H
# define PATH_FINDER_H


# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>

char	*get_cmd_path(const char *cmd, char **envp);

#endif
