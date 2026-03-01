/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operand.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:36:02 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 13:23:12 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
typedef struct s_op_token
{
	int			redir_ind;
	char		*cnt;
	t_quote_int	quotes[MAX_QUOTES_NUM];
	size_t		qpair_cnt;
}	t_op_token;

/* STDIN_FILENO always must be bonded with read-end;
 * STDOUT_FILENO always must be bonded with write-end
 *     name			- path to program with its arguments, redirections and etc;
 *     read_end		- stdin;
 *     write_end	- stdout;
 *     redirs		- redirections of this operand-program;
 *     red_cnt		- redirections counter;
 *     quotes		- all quotes pairs found in `name`;
 *     qpair_cnt	- counter of quote pairs;
 *     argv			- on heap;
 *     tokens		- tokens of this operand-program;
 *     my_env		- variables we created parsing this operand-program;
 *     f_per_cmd	- per-command flag for our variables */
typedef struct s_operand
{
	char		*name;
	int			read_end;
	int			write_end;
	t_redir		redirs[MAX_REDIRS_NUM];
	size_t		red_cnt;
	t_quote_int	quotes[MAX_QUOTES_NUM];
	size_t		qpair_cnt;
	char		**argv;
	int			argc;
	t_op_token	*tokens;
	size_t		token_cnt;
	t_env		*my_env;
	bool		f_per_cmd;
	pid_t		pid;
}	t_operand;

/* operand.c */
int		ops_init(t_operand *ops);
int		op_token_init(t_operand *op);
int		op_env_init(t_operand *op);

/* operand2.c */
void	ops_free(t_operand *ops);
void	operand_push_cleanup(t_operand *op);

#endif
