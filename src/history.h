#ifndef HISTORY_H
# define HISTORY_H

# include <stddef.h>

#include <readline/readline.h>
#include <readline/history.h>

# include "error.h"

# define MAX_HIST_LINES_NUM	4096

/* When unsetting both HISTSIZE and HISTFILESIZE,
 * they revert to their default values and history
 * continues to work. However, when HISTFILE is
 * unset, writing history to the file in the current
 * shell stops working. In a child shell, the value
 * of HISTFILE is reset to the default, and history
 * is written without any problems */
# define DEF_HISTSIZE		500
# define DEF_HISTFILESIZE	500
# define DEF_MSH_HIST_PATH	"~/.minishell_history"
# define DEF_BASH_HIST_PATH	"~/.bash_history"

/* Type of the history command.
 * Determines whether this
 * command was read from the
 * history file (in which case
 * it should not be written to
 * the history file again) or
 * entered during the current
 * session (in which case it
 * should be written to the
 * history file, since it is
 * a new command) */
typedef enum e_hist_type
{
	FROM_FILE,
	FROM_SESSION
}	t_hist_type;

/* line_num - number of line this command has
 *			  in history file. Commands from
 *			  current session have it as 0 */
typedef struct e_hist_cmd
{
	t_hist_type	type;
	char		*cmd;// on heap
	size_t		id;

}	t_hist_cmd;

typedef struct e_history
{
	t_hist_cmd	*lines;//on heap
	size_t		lines_num;
	char		*histfile_path;
	int			histsize;
	int			histfilesize;

}	t_history;

int		history_init(t_history *history);
int		history_push(t_history *history, char *line, t_hist_type type);
void	history_pop(t_history *history);
void	history_free(t_history *history);

#endif
