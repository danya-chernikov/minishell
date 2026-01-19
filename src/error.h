#ifndef ERROR_H
# define ERROR_H

# include <unistd.h>
# include <errno.h>
# include <stdbool.h>
# include <string.h>

# include "libft.h"

/* These values are returned by
 * most functions in most cases.
 *     COMMON_SYS_ERR - usually means a memory allocation
 *						error, or indicates a system call
 *						failure */
# define COMMON_SUCCESS			1
# define COMMON_FAILURE			0
# define COMMON_SYS_ERR			-1

# define ERR_BUF_SIZE			256

# define PARSER_ERR_MSG			"parsing error"
# define SYNTAX_ERR_MSG			"synxtax error"
# define INV_OPT_ERR_MSG		"invalid option"
# define C_OPT_ERR_MSG			"option requires an argument"
# define READONLY_VAR_ERR_MSG	"readonly variable"
# define PERM_DENIED_ERR_MSG	"Permission denied"
# define IS_DIR_ERR_MSG			"Is a directory"
# define CMD_NOT_FOUND_ERR_MSG	"command not found"
# define HOME_NOT_SET_ERR_MSG	"HOME not set"
# define NO_SUCH_FD_ERR_MSG		"No such file or directory"

# define MAX_ENV_NUM_ERR_MSG	"Too many environment variables were created"
# define MAX_ENV_NAME_ERR_MSG	"The variable name is too long"
# define MAX_ENV_VAL_ERR_MSG	"The variable value is too long"

# define GETPID_ERR_MSG			"Could not get my PID"
# define GETPPID_ERR_MSG		"Could not get my PPID"
# define GETUID_ERR_MSG			"Could not get my UID/EUID"
# define GETPWUID_ERR_MSG		"Could not get info about the user who is running me"

/* Common error our shell may return.
 *     CMD_LOCATED_BUT_NOT_EXEC_ERR - a file exists (it's path was
 *									  found), but does not have +x;
 *
 * */
typedef enum e_exit_code
{
	SUCCESS_CODE = 0,
	SYS_ERR = 1,
	CMD_BUILTIN_ERR = 2,
	SYNTAX_ERR = 2,
	CMD_LOCATED_BUT_NOT_EXEC_ERR = 126,
	CMD_NOT_LOCATED_ERR = 127
}	t_exit_code;

void	parser_error(bool *f_noerr);
void	print_sys_error(char *msg);
void	print_shell_error(char *culprit, char *msg);

#endif
