#ifndef SIGNALS_H
# define SIGNAL_H

# include "shell.h"
# include "libft.h"
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

void	handle_sigint_interactive(int sig);
void	init_signals_interactive(void);
void	init_signals_child(void);
void	init_signals_execution(void);

#endif
