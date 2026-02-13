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

/* Errors of parsing stage */
# define PARSER_ERR_MSG			"parsing error"
# define TOO_LONG_OP_ERR_MSG	"operand is too long"
# define TOO_MANY_OPS_ERR_MSG	"too many operands"
# define UNMATCH_QUOTES_ERR_MSG	"unmatched quotes"
# define EMPTY_PARS_ERR_MSG		"empty parentheses are not permitted"

/* Bash-like errors specific for shell */
# define SYNTAX_ERR_MSG			"syntax error"
# define INV_OPT_ERR_MSG		"invalid option"
# define C_OPT_ERR_MSG			"option requires an argument"
# define READONLY_VAR_ERR_MSG	"readonly variable"
# define PERM_DENIED_ERR_MSG	"Permission denied"
# define IS_DIR_ERR_MSG			"Is a directory"
# define CMD_NOT_FOUND_ERR_MSG	"command not found"
# define HOME_NOT_SET_ERR_MSG	"HOME not set"
# define NO_SUCH_FD_ERR_MSG		"No such file or directory"
# define DOM_TOO_LONG_ERR_MSG	"(Sub)domain is too long"
# define PROMPT_TOO_LONG		"Prompt is too long"

/* Redirection error */
# define REDIRECT_ERR_MSG		"syntax error near unexpected token" // When: $ cat <<
# define TOO_MANY_REDIRECTS		"too many redirections"
# define TOO_LONG_REDIRECT_PATH	"redirection's operand-path is too long"
# define TOO_LONG_HD_DELIM		"heredoc's delimiter is too long"
# define TOO_LONG_HD_CONTENT	"heredoc's content is too long"
# define AMBIG_REDIRECT_ERR_MSG	"ambiguous redirect"

/* Warnings */
# define HEREDOC_EOF_WARN_MSG	"here-document was delimited by end-of-file"

/* Overflow control */
# define MAX_ENV_NUM_ERR_MSG	"Too many environment variables were created"
# define MAX_ENV_NAME_ERR_MSG	"The variable name is too long"
# define MAX_ENV_VAL_ERR_MSG	"The variable value is too long"
# define MAX_OP_TOK_LEN_ERR_MSG	"The operand's token is too long"
# define MAX_OP_TOK_NUM_ERR_MSG	"Too many operand's tokens"
# define MAX_WC_FNUM_ERR_MSG	"Too many files in current directory"
# define MAX_WC_FN_LEN_ERR_MSG	"Filename is too long"

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
	CMD_NOT_LOCATED_ERR = 127,
	SIGNALED_CODE = 128
}	t_exit_code;

void	print_sys_error(char *msg);
void	print_shell_error(char *culprit, char *msg);
void	print_shell_warning(char *culprit, char *msg);

#endif
