#include "shell.h"
#include "engine.h"
#include "signals.h"

#include "debug.h"
#include "error.h"

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
int	msh_launch(t_shell *msh, int *ret_code)
{
	int	fres;

	if (msh->mode == NONINT_SCRIPT_MODE)
	{
		fres = launch_script(msh, ret_code);
	}
	else if (msh->mode == NONINT_CMD_MODE)
	{
		fres = launch_cmd(msh, ret_code);
	}
	else if (msh->mode == NONINT_STDIN_MODE)
	{
		fres = launch_stdin_cmd(msh, ret_code);
	}
	else if (msh->mode == INT_MODE)
	{
		fres = launch_int_session(msh, ret_code);	
	}
	return (fres);
}

int	launch_script(t_shell *msh, int *ret_code)
{
	char	*script_path;
	char	*line;
	int		gnlerr;
	int		fd;
	int		fres;

	*ret_code = EXIT_SUCCESS;
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
		free(script_path);
		return (CMD_NOT_LOCATED_ERR); // 127
	}
	if (access(script_path, R_OK) == -1) // If not readable
	{
		print_shell_error(msh->script, PERM_DENIED_ERR_MSG);
		free(script_path);
		return (CMD_LOCATED_BUT_NOT_EXEC_ERR); // 126
	}

	// Open and execute line by line
	fd = open(script_path, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		free(script_path);
		return (COMMON_SYS_ERR);
	}
	line = get_next_line(fd, &gnlerr);
	gnlerr = 0;
	while (line)
	{
		remove_newline(line);
		if (msh->opts.f_verbose)
			printf("%s\n", line);
		fres = shell_engine(msh, line, ret_code);
		free(line);
		if (fres != COMMON_SUCCESS)
		{
			gnl_finish(STDIN_FILENO);
			break ;
		}
		line = get_next_line(fd, &gnlerr);
	}
	if (!line && gnlerr)
	{
		print_shell_error(NULL, GNL_ERR_MSG);
		gnl_finish(fd);
		fres = COMMON_SYS_ERR;
	}
	if (close(fd) == -1)
	{
		perror("close");
		fres = COMMON_SYS_ERR;
	}
	free(script_path);
	return (fres);
}

/* Just execute the command 
 * that goes after -c and exit */
int	launch_cmd(t_shell *msh, int *ret_code)
{
	int	fres;

	*ret_code = EXIT_SUCCESS;
	fres = shell_engine(msh, msh->c_cmd, ret_code);
	return (fres);
}

/* Read commands from stdin and 
 * exexute them.
 * DO WE NEED TO PROCESS Ctrl+C by
 * ourselves here? */
int	launch_stdin_cmd(t_shell *msh, int *ret_code)
{
	int		fres;
	int		gnlerr;
	char	*line;

	fres = COMMON_SUCCESS;
	*ret_code = EXIT_SUCCESS;
	line = get_next_line(STDIN_FILENO, &gnlerr);
	gnlerr = 0;
	while (line)
	{
		remove_newline(line);
		if (msh->opts.f_verbose)
			printf("%s\n", line);
		fres = shell_engine(msh, line, ret_code);
		free(line);
		if (fres != COMMON_SUCCESS)
		{
			gnl_finish(STDIN_FILENO);
			return (fres);
		}
		line = get_next_line(STDIN_FILENO, &gnlerr);
	}
	if (!line && gnlerr)
	{
		print_shell_error(NULL, GNL_ERR_MSG);
		gnl_finish(STDIN_FILENO);
		return (COMMON_SYS_ERR);
	}
	return (fres);
}

/* Create an interactive shell session
 * and ask user for commands showing
 * prompt invitation */
int	launch_int_session(t_shell *msh, int *ret_code)
{
	int		fres;		// Function return code
	char	*rline_buf;

	// Set signals
	signals_init();	

	// Load history	
	if (msh_load_history(msh) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);

	fres = COMMON_SUCCESS;
	*ret_code = EXIT_SUCCESS;
	rline_buf = NULL;
	while (1)
	{
		// Form prompt invitation
		fres = gen_prompt_inv(msh);
		if (fres != COMMON_SUCCESS)
			return (fres);

		rl_done = 0;
		g_got_sigint = 0;
		rline_buf = readline(msh->prompt_inv);
		if (!rline_buf)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (g_got_sigint)
		{
			g_got_sigint = 0;
			*ret_code = SIGNALED_CODE + SIGINT; // 128 + 2 (SIGINT)
		}
		if (rline_buf[0] == '\0')
		{
			free(rline_buf);
			rline_buf = NULL;
			continue ;
		}

		add_history(rline_buf);

		if (msh->opts.f_verbose)
			printf("%s\n", rline_buf);
		fres = shell_engine(msh, rline_buf, ret_code);
		// In case if non-critial parser error occured
		// we just free `rline_buf` and prompt user again
		free(rline_buf);
		rline_buf = NULL;
		if (fres == COMMON_SYS_ERR)
			break ;
	} // while (1)

	return (fres);
}
