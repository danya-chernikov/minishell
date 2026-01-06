#ifndef OPERAND_H
# define OPERAND_H

# include <stddef.h>

# define READ_END		0
# define WRITE_END		1
# define DEFAULT_FD		-1
# define MAX_OPS_NUM	128 // Maximum number of operands

/* STDIN_FILENO always must be bonded with read-end;
 * STDOUT_FILENO always must be bonded with write-end */
typedef struct s_operand
{
	char	name[2];	// Path to program with its arguments
	int		read_end;	// stdin
	int		write_end;	// stdout
}	t_operand;

void	init_ops(t_operand *ops);

#endif
