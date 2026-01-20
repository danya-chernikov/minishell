#include "shell.h"
#include "engine.h"
#include "debug.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* If everything is OK, we return the exit code
 * of the last command executed by the shell
 * (regardless of which mode it was launched in).
 * If a system error occurs, we return -1
 * (COMMON_SYS_ERR). COMMON_SUCCESS is treated as
 * a normal command return value, so we don't
 * handle it specially. In main(), if the function
 * returns COMMON_SYS_ERR, we return 1, otherwise,
 * main() simply returns the exit code of the last
 * executed command, in any shell mode */
int	msh_launch(t_shell *msh)
{
	int	ret_code;

	if (msh->mode == NONINT_SCRIPT_MODE)
	{
		ret_code = launch_script(msh);
	}
	else if (msh->mode == NONINT_CMD_MODE)
	{
		ret_code = launch_cmd(msh);
	}
	else if (msh->mode == NONINT_STDIN_MODE)
	{
		ret_code = launch_stdin_cmd(msh);
	}
	else if (msh->mode == INT_MODE)
	{
		ret_code = launch_int_session(msh);
		// Load history	
		if (msh_load_history(msh) == COMMON_SYS_ERR)
			return (COMMON_SYS_ERR);

		// Form prompt
		
	}

	if (ret_code == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (ret_code);
}

int	launch_script(t_shell *msh)
{
	char	*script_path;
	char	*line;
	int		gnlerr;
	int		ret_code;
	int		fd;

	// Determine script's full path
	script_path = get_full_path(msh->script);
	if (!script_path)
		return (COMMON_SYS_ERR);
#if DEBUG == 1
	printf("%s\n", script_path);
#endif

	// Script must exist and be readable
	if (access(script_path, F_OK) == -1) // If not exists
	{
		print_shell_error(msh->script, NO_SUCH_FD_ERR_MSG);
		return (CMD_NOT_LOCATED_ERR); // 127
	}
	if (access(script_path, R_OK) == -1) // If not readable
	{
		print_shell_error(msh->script, PERM_DENIED_ERR_MSG);
		return (CMD_LOCATED_BUT_NOT_EXEC_ERR); // 126
	}

	// Open and execute line by line
	fd = open(script_path, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (COMMON_SYS_ERR);
	}
	line = get_next_line(fd, &gnlerr);
	while (line)
	{
		remove_newline(line);
		if (msh->opts.f_verbose)
			printf("%s\n", line);
		shell_engine(NULL, &ret_code);
		line = get_next_line(fd, &gnlerr);
	}
	if (!line && gnlerr)
	{
		print_shell_error(NULL, GNL_ERR_MSG);
		gnl_finish(fd);
		return (COMMON_SYS_ERR);
	}
	if (close(fd) == -1)
	{
		perror("close");
		return (COMMON_SYS_ERR);
	}
	return (ret_code);
}

int	launch_cmd(t_shell *msh)
{
	int	ret_code;

	(void)msh;
	ret_code = 0;
	return (ret_code);
}

int	launch_stdin_cmd(t_shell *msh)
{
	int	ret_code;

	(void)msh;
	ret_code = 0;
	return (ret_code);
}

int	launch_int_session(t_shell *msh)
{
	int	ret_code;

	(void)msh;
	ret_code = 0;
	return (ret_code);
}
