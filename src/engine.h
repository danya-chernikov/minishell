#ifndef ENGINE_H
# define ENGINE_H

# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <unistd.h> // Do we need it here?

# include "parser.h"
# include "shell.h"
# include "debug.h"

# define MAX_SUBSHS_NUM	128 // Maximum number of subshells

/* cmds		- commands to execute inside shell;
 * posargv	- positional arguments;
 * pos_argc - number of positional arguemnts;
 * settings	- shell settings;
 * env		- inherited environment.
 *
 * NONINT_SCRIPT (We want our shell execute a script)
 *     cmd=NULL;
 *     mode=NONINT_SCRIPT;
 * NONINT_CMD
 *     script_path=NULL;
 *     mode=NONINT_CMD;
 * INT_LOG
 *     cmds=NULL;
 *     script_path=NULL;
 *     pos_argv=NULL;
 *     pos_argc=0;
 *     mode=INT_LOG;
 * INT_NONLOG
 *     cmds=NULL;
 *     script_path=NULL;
 *     pos_argv=NULL;
 *     pos_argc=0;
 *     mode=INT_NONLOG.
 * */
typedef struct s_engine_params
{
	char			*cmds;
	char			**env;
	char			*script_path;
	char			**pos_argv;
	size_t			pos_argc;
	t_settings		*settings;
	t_shell_mode	mode;
}	t_engine_params;

int	exec_ops(t_parser_data *d);
int	close_pipes(t_parser_data *d);

#endif
