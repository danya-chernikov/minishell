#ifndef OPERAND_H
# define OPERAND_H

# include "redirect.h"
# include "quote.h"

# include <stddef.h>

# define READ_END			0
# define WRITE_END			1
# define DEFAULT_FD			-1
# define MAX_OPS_NUM		1024	// Maximum number of operands
# define MAX_OP_LEN			1024	// Maximum operand length
# define MAX_OP_TOKENS_NUM	1024	// Maximum operand tokens number
# define MAX_OP_TOKEN_LEN	256		// Maximum token length
# define MAX_ARGC_NUM		1024
# define MAX_ARGV_LEN		4096

typedef struct s_env	t_env;

/* Type of quote to which the
 * operand's token indices belong */
typedef enum e_ind_type
{
	IND_QDOUBLE,
	IND_QSINGLE,
	IND_QNONE

}	t_ind_type;

/* Operand's token.
 *     cnt			- token's content;
 *
 *     redir_ind	- the redirection index of this token
 *					  in redirections array. We need it so
 *					  we could update the corresponding
 *					  redirection */
typedef struct	s_op_token
{
	int			redir_ind;
	char		*cnt;
	t_quote_int	quotes[MAX_QUOTES_NUM];
	size_t		qpair_cnt;

}	t_op_token;

/* STDIN_FILENO always must be bonded with read-end;
 * STDOUT_FILENO always must be bonded with write-end */
typedef struct s_operand
{
	char		*name;					// Path to program with its arguments, redirections and etc.

	int			read_end;				// stdin
	int			write_end;				// stdout
	
	t_redir		redirs[MAX_REDIRS_NUM]; // Redirections of this operand-program
	size_t		red_cnt;				// Redirections counter

	t_quote_int quotes[MAX_QUOTES_NUM];	// All quotes pairs found in `name`
	size_t		qpair_cnt;				// Counter of quote pairs

	char		**argv;					// On heap
	int			argc;

	t_op_token	*tokens;				// Tokens of this operand-program
	size_t		token_cnt;

	t_env		*my_env;				// Variables we created parsing this operand-program
	bool		f_per_cmd;				// Per-command flag for our variables

	pid_t		pid;

}	t_operand;

int		ops_init(t_operand *ops);
int		op_token_init(t_operand *op);
int		op_env_init(t_operand *op);
void	ops_free(t_operand *ops);

#endif
