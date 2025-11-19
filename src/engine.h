#ifndef ENGINE_H
# define ENGINE_H

# include "shell.h"

# define SEARCH_DEPTH	20

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

int	engine(t_engine_params *params);

#endif
