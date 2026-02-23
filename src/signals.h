#ifndef SIGNALS_H
# define SIGNALS_H

#include <signal.h>

# define SIGQUIT_MSG	"Quit (core dumped)\n"

extern volatile sig_atomic_t	g_got_sigint;

/* signals.c */
void	signals_init(void);
void	sigquit_handler(int signo);
void	sigint_handler(int signo);
void	sigwinch_handler(int signo);

/* signals2.c */
void	heredoc_sigint_handler(int signo);
void	child_set_heredoc_signals(void);
void	parent_ignore_sigint_sigquit(struct sigaction *old_int,
		struct sigaction *old_quit);
void	parent_restore_signals(struct sigaction *old_int,
		struct sigaction *old_quit);

#endif
