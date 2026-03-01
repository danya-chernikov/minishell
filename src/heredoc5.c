/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:27:25 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:27:26 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "operand.h"
#include "shell.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

int	hd_fork_failed(int p[2], bool flags[2],
		struct termios *saved, struct sigaction old[2])
{
	perror("fork");
	close(p[HD_READ]);
	close(p[HD_WRITE]);
	hd_restore_parent(flags, saved, old);
	return (COMMON_SYS_ERR);
}
