#ifndef OPERAND_H
# define OPERAND_H

# include "redirect.h"

# include <stddef.h>

# define READ_END		0
# define WRITE_END		1
# define DEFAULT_FD		-1
# define MAX_OPS_NUM	256		// Maximum number of operands
# define MAX_OP_LEN		1024	// Maximum operand length

/* STDIN_FILENO always must be bonded with read-end;
 * STDOUT_FILENO always must be bonded with write-end */
typedef struct s_operand
{
	char		*name;					// Path to program with its arguments
	int			read_end;				// stdin
	int			write_end;				// stdout
	t_redir		redirs[MAX_REDIRS_NUM]; // Redirections of this operand-program
	size_t		red_cnt;				// Redirections counter
	pid_t		pid;
	char		**argv;

}	t_operand;

int		ops_init(t_operand *ops);
void	ops_free(t_operand *ops);

#endif
