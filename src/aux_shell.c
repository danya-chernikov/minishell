#include "shell.h"
#include "aux_common.h"

#include <stdlib.h>

static int	get_username(char **username);
static int	get_subdomain(char **subdomain);
static int	form_invitation(t_shell *msh, char *subdomain,
		char *username, char *pwd);

/* Generates prompt invitation user@host:pwd$ */
int	gen_prompt_inv(t_shell *msh)
{
	char	*subdomain;
	char	*username;
	char	*pwd;
	int		fres;

	fres = get_subdomain(&subdomain);
	if (fres != COMMON_SUCCESS)
		return (fres);
	pwd = msh->env.vars[SE_PWD].value;
	fres = get_username(&username);
	if (fres != COMMON_SUCCESS)
		return (fres);
	fres = form_invitation(msh, username, subdomain, pwd);
	free(username);
	free(subdomain);
	return (COMMON_SUCCESS);
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
	size_t	sd_len;
	char	*point;
	char	*hostname;

	*subdomain = (char *)malloc(MAX_SUBDOMAIN_LEN * sizeof (char));
	if (!(*subdomain))
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	hostname = get_hostname();
	point = ft_strchr(hostname, '.');
	if (point)
	{
		sd_len = (ptrdiff_t)point - (ptrdiff_t)hostname;
		if (sd_len > MAX_SUBDOMAIN_LEN - 1)
		{
			print_shell_error(NULL, DOM_TOO_LONG_ERR_MSG);
			free(hostname);
			return (COMMON_FAILURE);
		}
		ft_strlcpy(*subdomain, hostname, sd_len + 1);
	}
	else
		ft_strlcpy(*subdomain, hostname, MAX_SUBDOMAIN_LEN);
	free(hostname);
	return (COMMON_SUCCESS);
}

static int	form_invitation(t_shell *msh, char *subdomain,
		char *username, char *pwd)
{
	size_t	inv_len;

	inv_len = ft_strlen(username) + ft_strlen(subdomain) + ft_strlen(pwd) + 5;
	if (inv_len > PROMPT_INV_LEN - 1)
		return (print_shell_error(NULL, DOM_TOO_LONG_ERR_MSG), COMMON_FAILURE);
	if (msh->prompt_inv)
		free(msh->prompt_inv);
	msh->prompt_inv = (char *)malloc(inv_len * sizeof(char));
	if (!msh->prompt_inv)
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	ft_strlcpy(msh->prompt_inv, username, inv_len);
	ft_strlcat(msh->prompt_inv, "@", inv_len);
	ft_strlcat(msh->prompt_inv, subdomain, inv_len);
	ft_strlcat(msh->prompt_inv, ":", inv_len);
	ft_strlcat(msh->prompt_inv, pwd, inv_len);
	ft_strlcat(msh->prompt_inv, "$ ", inv_len);
	return (COMMON_SUCCESS);
}

void	msh_update_retcode(t_shell *msh, int status)
{
	char	*new_val;

	new_val = ft_itoa(status);
	if (!new_val)
		return ;
	if (msh->env.vars[PV_RETCODE].value)
		free(msh->env.vars[PV_RETCODE].value);
	msh->env.vars[PV_RETCODE].value = new_val;
}
