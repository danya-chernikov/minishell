/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:39:06 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:39:07 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "libft.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <readline/readline.h>

volatile sig_atomic_t	g_got_sigint = 0;

void	signals_init(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = sigwinch_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGWINCH, &sa, NULL);
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
	write(STDOUT_FILENO, "^C", 3);
	g_got_sigint = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* The best way to avoid output mixing */
void	sigwinch_handler(int signo)
{
	(void)signo;
	rl_resize_terminal();
}
