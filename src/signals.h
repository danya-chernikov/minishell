#ifndef SIGNALS_H
# define SIGNALS_H

#include <signal.h>

extern volatile sig_atomic_t	g_got_sigint;

void	signals_init(void);
void	sigquit_handler(int signo);
void	sigint_handler(int signo);
void	sigwinch_handler(int signo);

#endif
