#include "signals.h"
#include "libft.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	g_got_sigint = 0;

void	signals_init(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;

	sigemptyset(&sa.sa_mask);

	// SIGINT
	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

	// SIGQUIT	
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa, NULL);
}

/* A handler that prints something
 * without using rl_on_new_line() */
void	sigquit_handler(int signo)
{
	(void)signo;
}

/* A handler that prints something and
 * then calls rl_on_new_line() */
void	sigint_handler(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "^C\n", 3);
	g_got_sigint = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* The best way to avoid output mixing */
void	sigwinch_handler(int signo)
{
	(void)signo;
}
