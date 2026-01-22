#ifndef BUILTIN_H
# define BUILTIN_H

//#include "libft.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int	ft_pwd(void);
int	ft_echo(char **args);
int	ft_cd(char **args, char **envp);
int	ft_env(char **envp);
int	ft_exit(char **args);

#endif
