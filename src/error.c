/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:39:39 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 13:40:10 by jhvalenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "shell.h"

void	print_sys_error(char *msg)
{
	char	err_buf[ERR_BUF_SIZE];
	char	*err;

	err = strerror(errno);
	ft_strlcpy(err_buf, msg, ERR_BUF_SIZE);
	ft_strlcat(err_buf, ": ", ERR_BUF_SIZE);
	ft_strlcat(err_buf, err, ERR_BUF_SIZE);
	ft_strlcat(err_buf, "\n", ERR_BUF_SIZE);
	write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
}

void	print_shell_error(char *culprit, char *msg)
{
	char	err_buf[ERR_BUF_SIZE];

	ft_strlcpy(err_buf, MSH_NAME_SHORT, ERR_BUF_SIZE);
	ft_strlcat(err_buf, ": ", ERR_BUF_SIZE);
	if (culprit)
	{
		ft_strlcat(err_buf, culprit, ERR_BUF_SIZE);
		ft_strlcat(err_buf, ": ", ERR_BUF_SIZE);
	}
	ft_strlcat(err_buf, msg, ERR_BUF_SIZE);
	ft_strlcat(err_buf, "\n", ERR_BUF_SIZE);
	write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
}

void	print_shell_warning(char *culprit, char *msg)
{
	char	warn_buf[ERR_BUF_SIZE];

	ft_strlcpy(warn_buf, MSH_NAME_SHORT, ERR_BUF_SIZE);
	ft_strlcat(warn_buf, ": warning: ", ERR_BUF_SIZE);
	if (culprit)
	{
		ft_strlcat(warn_buf, culprit, ERR_BUF_SIZE);
		ft_strlcat(warn_buf, ": ", ERR_BUF_SIZE);
	}
	ft_strlcat(warn_buf, msg, ERR_BUF_SIZE);
	ft_strlcat(warn_buf, "\n", ERR_BUF_SIZE);
	write(STDERR_FILENO, warn_buf, ft_strlen(warn_buf));
}
