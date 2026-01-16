#ifndef REDIRECT_H
# define REDIRECT_H

# include "libft.h"
# include "path_finder.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>

int     redirect_input(char *filename);
int	redirect_output(char *filename);
int	redirect_append(char *filename);

#endif
