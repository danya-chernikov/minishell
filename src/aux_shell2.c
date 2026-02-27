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
