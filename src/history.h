#ifndef HISTORY_H
#define HISTORY_H

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
	size_t		line_num;
	t_vector	cmd;
	t_hist_type	type;
}	t_hist_cmd;

#endif
