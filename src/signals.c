#include "signals.h"

#if 0
SIGNAL HANDLING MODULE
This module manages how the process responds to external signals 
(like Ctrl+C or Ctrl+\) depending on the current state of the shell:
1. Interactive Mode: Standard prompt behavior.
2. Child Process: Default Linux termination behavior.
3. Parent Execution: Ignoring signals while waiting for children.
#endif
static void	error_msg(void)
{
	const char	*str_error;

	str_error = strerror(errno);
	write(STDERR_FILENO, str_error, ft_strlen(str_error));
	write(STDERR_FILENO, "\n", 1);
}

#if 0
handle_sigint_interactive - Signal handler for SIGINT (Ctrl+C).
Redraws the Readline prompt on a new line without exiting.
#endif

void	handle_sigint_interactive(int sig)
{
	(void)sig;

	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

#if 0
init_signals_interactive - Configures signals for the main shell loop.
- SIGINT (Ctrl+C): Handled by handle_sigint_interactive.
- SIGQUIT (Ctrl+\): Ignored.
#endif
void	init_signals_interactive(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &handle_sigint_interactive;
	sa.sa_flags = SA_RESTART;
	rl_catch_signals = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		error_msg();
		return ;
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		error_msg();
		return ;
	}
}

#if 0
CHILD MODE: This is called INSIDE the fork, before execve.
It restores default Linux behavior:
- Ctrl+C -> Terminates the process.
- Ctrl+\ -> Terminates the process (Core Dump).
#endif
void	init_signals_child(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		error_msg();
		return ;
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		error_msg();
		return ;
	}
}

#if 0
MODO EJECUCIÓN (PADRE): Mientras esperamos al hijo.
Ignoramos las señales porque el hijo ya las recibirá y morirá si toca.
Nosotros solo esperamos.
#endif
void	init_signals_execution(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		error_msg();
		return ;
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		error_msg();
		return ;
	}
}
