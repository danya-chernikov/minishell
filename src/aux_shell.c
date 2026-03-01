/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:48:27 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 02:00:44 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "aux_common.h"

#include <stdlib.h>

static int	get_username(char **username);
static int	get_subdomain(char **subdomain);
static int	extract_subdomain(char **subdomain, char *hostname, char *point);
static int	form_invitation(t_shell *msh, char *subdomain,
				char *username, char *pwd);

/* Generates prompt invitation user@host:pwd$ */
int	gen_prompt_inv(t_shell *msh)
{
	char	*subdomain;
	char	*username;
	char	*pwd;
	int		fres;

	subdomain = NULL;
	username = NULL;
	fres = get_subdomain(&subdomain);
	if (fres != COMMON_SUCCESS)
		return (fres);
	fres = get_username(&username);
	if (fres != COMMON_SUCCESS)
	{
		free(subdomain);
		return (fres);
	}
	pwd = msh->env.vars[SE_PWD].value;
	fres = form_invitation(msh, subdomain, username, pwd);
	free(username);
	free(subdomain);
	return (fres);
}

static int	get_username(char **username)
{
	uid_t		uid;
	t_passwd	pwd;
	int			fres;

	fres = ft_getuid(&uid);
	if (fres == -1)
		return (COMMON_SYS_ERR);
	fres = ft_getpwuid(&pwd, uid);
	if (fres != COMMON_SUCCESS)
		return (print_shell_error(NULL, USER_NOT_FOUND), COMMON_FAILURE);
	*username = ft_strdup(pwd.pw_name);
	free_pwd(&pwd);
	if (!(*username))
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* Get first first level subdomain (or what it is..)
 * sd_len - subdomain length; */
static int	get_subdomain(char **subdomain)
{
	char	*point;
	char	*hostname;

	*subdomain = (char *)malloc(MAX_SUBDOMAIN_LEN * sizeof (char));
	if (!(*subdomain))
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	hostname = get_hostname();
	if (!hostname)
	{
		free(*subdomain);
		return (COMMON_SYS_ERR);
	}
	point = ft_strchr(hostname, '.');
	if (point)
	{
		if (extract_subdomain(subdomain, hostname, point) == COMMON_FAILURE)
			return (COMMON_FAILURE);
	}
	else
		ft_strlcpy(*subdomain, hostname, MAX_SUBDOMAIN_LEN);
	free(hostname);
	return (COMMON_SUCCESS);
}

static int	extract_subdomain(char **subdomain, char *hostname, char *point)
{
	size_t	sd_len;

	sd_len = (ptrdiff_t)point - (ptrdiff_t)hostname;
	if (sd_len > MAX_SUBDOMAIN_LEN - 1)
	{
		print_shell_error(NULL, DOM_TOO_LONG_ERR_MSG);
		return (free(hostname), free(*subdomain), COMMON_FAILURE);
	}
	ft_strlcpy(*subdomain, hostname, sd_len + 1);
	return (COMMON_SUCCESS);
}

static int	form_invitation(t_shell *msh, char *subdomain,
		char *username, char *pwd)
{
	size_t	inv_len;
	char	*new_prompt;

	inv_len = ft_strlen(username) + ft_strlen(subdomain) + ft_strlen(pwd) + 5;
	if (inv_len > PROMPT_INV_LEN - 1)
		return (print_shell_error(NULL, DOM_TOO_LONG_ERR_MSG), COMMON_FAILURE);
	new_prompt = (char *)malloc(inv_len * sizeof(char));
	if (!new_prompt)
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	ft_strlcpy(new_prompt, username, inv_len);
	ft_strlcat(new_prompt, "@", inv_len);
	ft_strlcat(new_prompt, subdomain, inv_len);
	ft_strlcat(new_prompt, ":", inv_len);
	ft_strlcat(new_prompt, pwd, inv_len);
	ft_strlcat(new_prompt, "$ ", inv_len);
	if (msh->prompt_inv)
		free(msh->prompt_inv);
	msh->prompt_inv = new_prompt;
	return (COMMON_SUCCESS);
}
