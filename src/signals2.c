/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:28:38 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 13:29:15 by jhvalenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

#include <unistd.h>
#include <readline/readline.h>

void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	g_got_sigint = 1;
	rl_done = 1;
	write(1, "\n", 1);
}

void	child_set_heredoc_signals(void)
{
	struct sigaction	sa;	

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	parent_ignore_sigint_sigquit(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	struct sigaction	sa;	

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old_int);
	sigaction(SIGQUIT, &sa, old_quit);
}

void	parent_restore_signals(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}
