/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 13:16:17 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/25 13:16:19 by jhvalenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "shell.h"
#include "operand.h"
#include "env.h"

#include "error.h"
#include "libft.h"

#include <linux/limits.h>

static int	update_pwd_vars(t_shell *msh, char *oldpwd);

int	builtin_cd(t_shell *msh, t_operand *op)
{
	char	*dest;
	char	*oldpwd;
	int		fret;

	dest = NULL;
	if (op->argc < 2)
		dest = msh->env.vars[PV_HOME].value;
	else
		dest = op->argv[1];
	if (!dest)
	{
		print_shell_error(NULL, HOME_NOT_SET_ERR_MSG);
		return (CMD_BUILTIN_ERR);
	}
	oldpwd = env_get_val(&msh->env, "PWD");
	if (chdir(dest) == -1)
		return (perror_and_return(dest, RET_CMD_FAILURE));
	if (oldpwd)
		fret = update_pwd_vars(msh, oldpwd);
	else
		fret = update_pwd_vars(msh, "");
	if (fret != COMMON_SUCCESS)
		return (CMD_BUILTIN_ERR);
	return (RET_CMD_SUCCESS);
}

static int	update_pwd_vars(t_shell *msh, char *oldpwd)
{
	char	buf[PATH_MAX];
	char	*pwd;
	int		fret;

	if (!getcwd(buf, sizeof (buf)))
		return (perror_and_return("getcwd", COMMON_SYS_ERR));
	pwd = ft_strdup(buf);
	if (!pwd)
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	if (oldpwd)
	{
		fret = env_set(&msh->env, ft_strdup("OLDPWD"), ft_strdup(oldpwd), ENV);
		if (fret != COMMON_SUCCESS)
		{
			if (fret == COMMON_SYS_ERR)
				perror("malloc");
			return (free_one_and_return(pwd, fret));
		}
	}
	fret = env_set(&msh->env, ft_strdup("PWD"), pwd, ENV);
	if (fret == COMMON_SYS_ERR)
		perror("malloc");
	return (fret);
}
