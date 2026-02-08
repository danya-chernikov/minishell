#include "exec.h"
#include "shell.h"
#include "operand.h"
#include "prompt_parser.h"

#include "error.h"

#include <stdio.h>
#include <unistd.h>

int	close_pipes(t_shell *msh)
{
	size_t	i;

	i = 0;
	while (i < msh->pd->pipe_cnt)
	{
		if (close(msh->pd->pipes[i][READ_END]) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
		if (close(msh->pd->pipes[i][WRITE_END]) == -1)
		{
			perror("close");
			return (COMMON_SYS_ERR);
		}
		++i;
	}
	return (COMMON_SUCCESS);
}
