/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_shell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:04:03 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/03 18:55:46 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "colors.h"

#include "libft.h"

#include <stdlib.h>

/* Concatenates prompt components and stores the result in `new_prompt`*/
void	concat_prompt_comps_first(char *new_prompt, char *username,
			char *subdomain, size_t inv_len)
{
	ft_strlcpy(new_prompt, CL_GREEN, inv_len);
	ft_strlcat(new_prompt, CL_BOLD, inv_len);
	ft_strlcat(new_prompt, username, inv_len);
	ft_strlcat(new_prompt, "@", inv_len);
	ft_strlcat(new_prompt, subdomain, inv_len);
	ft_strlcat(new_prompt, CL_RESET, inv_len);
}

void	concat_prompt_comps_second(char *new_prompt, char *pwd, size_t inv_len)
{
	ft_strlcat(new_prompt, ":", inv_len);
	ft_strlcat(new_prompt, CL_BLUE, inv_len);
	ft_strlcat(new_prompt, CL_BOLD, inv_len);
	ft_strlcat(new_prompt, pwd, inv_len);
	ft_strlcat(new_prompt, CL_RESET, inv_len);
	ft_strlcat(new_prompt, "$ ", inv_len);
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
