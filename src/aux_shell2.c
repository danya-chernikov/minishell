/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_shell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:04:03 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:04:04 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#include <stdlib.h>

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
