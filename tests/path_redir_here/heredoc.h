#ifndef HEREDOC_H
# define HEREDOC_H

# include "libft.h"

# include <string.h>
# include <fcntl.h>   // O_CREAT, O_RDWR, etc.
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <errno.h>
# include <stdlib.h>

int	get_heredoc_fd(char *delimiter);

#endif
