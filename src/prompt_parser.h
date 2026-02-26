/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_parser.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:52:07 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 02:20:17 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_PARSER_H
# define PROMPT_PARSER_H

# include <stdlib.h>

# include "par.h"
# include "quote.h"
# include "token.h"
# include "operand.h"
# include "aux_common.h"
# include "error.h"

# define MAX_FORMAT_STR_LEN	256
# define MAX_PIPES_NUM		256
# define MAX_PROGRAM_PATH	4096
# define NONE_INDEX			-1
# define NONE_PIPE			-1

/* Describes first-level token parser data.
 * Main definitions:
 *     prompt		- prompt string entered by user;
 *     pi			- prompt index;
 *     op_cnt		- operand counter;
 *     ops			- operands (programs to launch);
 *     token_cnt	- token counter;
 *     tokens		- here we store all tokens we found during parsing;
 *     quotes		- all quotes pairs found in the prompt;
 *     qpair_cnt	- counter of quote pairs;
 * Auxiliary definitions:
 *     opar_num		- number of all opening-parentheses;
 *	   all_open_pars- indexes of all opening-parentheses;
 *	   opar_cnt		- opening-parentheses counter (must be signed type);
 *	   open_par		- opening-parentheses indexes found and thier flags;
 *	   cpar_cnt		- closing-parentheses counter (for now let it be int);
 *     close_par	- closing-parentheses indexes found and their flags;
 *     pars			- a member that represents each parentheses pair;
 *     par_cnt		- parentheses pair counter */
typedef struct s_parser_data
{
	char		*prompt;
	size_t		pi;
	size_t		op_cnt;
	t_operand	ops[MAX_OPS_NUM];
	size_t		token_cnt;
	t_token		tokens[MAX_TOKENS_NUM];
	t_quote_int	quotes[MAX_QUOTES_NUM];
	size_t		qpair_cnt;
	size_t		opar_num;
	size_t		all_open_pars[MAX_PAR_NUM][2];	
	t_ll		opar_cnt;
	size_t		open_par[MAX_PAR_NUM];	
	t_ll		cpar_cnt;
	size_t		close_par[MAX_PAR_NUM][2];
	t_pair		pars[MAX_PAR_NUM];
	size_t		par_cnt;
}	t_parser_data;

/* prompt_parser.c */
int		parser_init(t_parser_data *d, char *rline_buf);
void	parser_free(t_parser_data *d);
int		parser_engine(t_parser_data *d);
int		parser_engine_loop_body(t_parser_data *d, int *opar_ind, size_t plen);

/* prompt_parser_proc_spec_char.c */
int		process_special_char(t_parser_data *d, size_t prompt_len);
int		process_special_char_check_pars(t_parser_data *d);

/* prompt_parser_proc_spec_char2.c */
int		process_pipe_and_logical_operators(t_parser_data *d, size_t prompt_len);
int		process_special_char_handle_pipe(t_parser_data *d);
int		process_special_char_handle_and(t_parser_data *d);
int		process_special_char_handle_or(t_parser_data *d);

/* prompt_parser_proc_nonspec_char.c */
int		process_nonspecial_char(t_parser_data *d, size_t prompt_len);
int		process_nonspecial_char_prep(t_parser_data *d, size_t prompt_len);
int		process_nonspecial_char_handle_pipe(t_parser_data *d, size_t plen);
int		process_nonspecial_char_handle_nonpipe(t_parser_data *d, size_t plen);

/* prompt_parser2.c */
int		handle_open_par(t_parser_data *d, int opar_ind);
int		handle_open_par_check_errors(t_parser_data *d, size_t *lpi, size_t pl);
int		handle_close_par(t_parser_data *d);

/* prompt_parser3.c */
void	token_push(t_parser_data *d, t_token_type type);
int		operand_push(t_parser_data *d, size_t pi);
bool	check_empty_par(t_parser_data *d);
int		later_goes_open_par(char *str, size_t ind);

/* prompt_parser4.c */
bool	its_logical_and(char *prompt, size_t plen, size_t pi);
bool	its_logical_or(char *prompt, size_t plen, size_t pi);
bool	its_pipe(char *prompt, size_t plen, size_t pi);
void	skip_spaces(char *prompt, size_t *pi);

/* prompt_parser5.c */
bool	is_spec_char(char sym);
bool	is_inside_quotes(t_parser_data *d, size_t pi);
bool	is_special_char_outside_quotes(t_parser_data *d, size_t pi);
void	remove_right_spaces(char *prompt);
void	remove_left_spaces(char *prompt);

#endif
