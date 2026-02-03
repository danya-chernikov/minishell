#include "shell.h"
#include "engine.h"
#include "debug.h"
#include "builtin.h" // for exit command

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
		//shell_engine(line, &ret_code);
		shell_engine(msh, line, &ret_code);
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

/* Just execute the command 
 * that goes after -c and exit */
int	launch_cmd(t_shell *msh)
{
	int	ret_code;

	ret_code = 0;
	//shell_engine(msh->c_cmd, &ret_code);
	shell_engine(msh, msh->c_cmd, &ret_code);
	return (ret_code);
}

/* Read commands from stdin and 
 * exexute them */
int	launch_stdin_cmd(t_shell *msh)
{
	int		ret_code;
	int		gnlerr;
	char	*line;

	ret_code = 0;
	line = get_next_line(STDIN_FILENO, &gnlerr);
	while (line)
	{
		remove_newline(line);
		if (msh->opts.f_verbose)
			printf("%s\n", line);
		//shell_engine(line, &ret_code);
		shell_engine(msh, line, &ret_code);
		line = get_next_line(STDIN_FILENO, &gnlerr);
	}
	if (!line && gnlerr)
	{
		print_shell_error(NULL, GNL_ERR_MSG);
		gnl_finish(STDIN_FILENO);
		return (COMMON_SYS_ERR);
	}
	return (ret_code);
}

/* Create an interactive shell session
 * and ask user for commands showing
 * prompt invitation */
int	launch_int_session(t_shell *msh)
{
	int		ret_code;	// Shell return code
	int		fres;		// Function return code
	char	*rline_buf;

	// Load history	
	if (msh_load_history(msh) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);

	ret_code = 0;
	rline_buf = NULL;
	while (1)
	{
		// Form prompt invitation
		fres = gen_prompt_inv(msh);
		if (fres != COMMON_SUCCESS)
			return (fres);

		rline_buf = readline(msh->prompt_inv);
		// Manejo de Ctrl+D (EOF) -> NULL
		if (rline_buf == NULL)
		{
			write(STDERR_FILENO, "exit\n", ft_strlen("exit\n"));
			break ;
		}
		if (ft_strlen(rline_buf) > 0)
		{
			add_history(rline_buf);
			if (shell_engine(msh, rline_buf, &ret_code) == COMMON_SYS_ERR)
			{
				free(rline_buf);
				return (COMMON_SYS_ERR);
			}
		}
		#if 0
		if (ft_strlen(rline_buf) == 0)
		{
			free(rline_buf);
			rline_buf = NULL;
			continue;
		}
		add_history(rline_buf);
		if (strings_equal(rline_buf, EXIT_CMD))
		{
			free(rline_buf);
			rline_buf = NULL;
			break;
		}
		if (shell_engine(rline_buf, &ret_code) == COMMON_SYS_ERR) // Critial system error occured
			return (COMMON_SYS_ERR);
		#endif
		// In case if non-critial parser error occured
		// we just free `rline_buf` and prompt user again
		free(rline_buf);
		rline_buf = NULL;
	}
	return (ret_code);
}
