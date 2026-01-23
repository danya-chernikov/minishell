#include "shell.h"

#include <stdlib.h>

/* Generates prompt invitation user@host:pwd$ */
int	gen_prompt_inv(t_shell *msh)
{
	size_t	inv_len;
	char	*hostname;
	char	*username;
	char	*pwd;
	size_t	sd_len;	// subdomain length
	char	subdomain[MAX_SUBDOMAIN_LEN];
	char	*point;

	// Let's recall our machine's hostname
	hostname = msh->env.vars[SL_HOSTNAME].value;
	// Get first first level subdomain (or what it is..)
	point = ft_strchr(hostname, '.');
	if (point) // There is a point in the domain name
	{
		sd_len = ft_abs((ptrdiff_t)hostname - (ptrdiff_t)point);
		if (sd_len > MAX_SUBDOMAIN_LEN - 1)
		{
			print_shell_error(NULL, DOM_TOO_LONG_ERR_MSG);
			return (COMMON_FAILURE);
		}
		ft_strlcpy(subdomain, hostname, sd_len + 1);
	}
	else
		ft_strlcpy(subdomain, hostname, MAX_SUBDOMAIN_LEN);
	// Get username
	username = msh->env.vars[SE_USER].value;
	// Get $PWD value
	pwd = msh->env.vars[SE_PWD].value;
	inv_len = ft_strlen(username) + ft_strlen(subdomain) + ft_strlen(pwd) + 5;
	if (inv_len > PROMPT_INV_LEN - 1)
	{
		print_shell_error(NULL, DOM_TOO_LONG_ERR_MSG);
		return (COMMON_FAILURE);
	}
	msh->prompt_inv = (char *)malloc(inv_len * sizeof(char));
	if (!msh->prompt_inv)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	ft_strlcpy(msh->prompt_inv, username, inv_len);
	ft_strlcat(msh->prompt_inv, "@", inv_len);
	ft_strlcat(msh->prompt_inv, subdomain, inv_len);
	ft_strlcat(msh->prompt_inv, ":", inv_len);
	ft_strlcat(msh->prompt_inv, pwd, inv_len);
	ft_strlcat(msh->prompt_inv, "$ ", inv_len);
	return (COMMON_SUCCESS);
}
