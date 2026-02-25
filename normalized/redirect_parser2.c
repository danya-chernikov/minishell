#include "redirect_parser.h"
#include "operand.h"

#include <stdlib.h>

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* Handles input redirection '<'. Allocates path memory
 * and updates the operand's redirection array.*/
int	add_redir_in(t_operand *op, size_t *op_i)
{
	size_t	wi;

	if (validate_redir(op, op_i, 1) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op->redirs[op->red_cnt].type = REDIR_IN;
	op->redirs[op->red_cnt].target_fd = STDIN_FILENO;
	wi = *op_i + 1;
	skip_spaces(op->name, &wi);
	if ((op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	op->redirs[op->red_cnt].path = (char *)malloc(PATH_MAX * sizeof(char));
	if (!op->redirs[op->red_cnt].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	if (extr_redir_path(op, &wi, op->redirs[op->red_cnt].path, PATH_MAX) == -1)
		return (COMMON_FAILURE);
	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

/* Handles output redirection '>'. Similar logic to input
 * but targets STDOUT_FILENO.*/
int	add_redir_out(t_operand *op, size_t *op_i)
{
	size_t	wi;

	if (validate_redir(op, op_i, 1) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op->redirs[op->red_cnt].type = REDIR_OUT;
	op->redirs[op->red_cnt].target_fd = STDOUT_FILENO;
	wi = *op_i + 1;
	skip_spaces(op->name, &wi);
	if ((op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	op->redirs[op->red_cnt].path = (char *)malloc(PATH_MAX * sizeof(char));
	if (!op->redirs[op->red_cnt].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	if (extr_redir_path(op, &wi, op->redirs[op->red_cnt].path, PATH_MAX) == -1)
		return (COMMON_FAILURE);
	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

/* Handles append redirection '>>'. Skips two characters
 *and sets type to REDIR_APP.*/
int	add_redir_app(t_operand *op, size_t *op_i)
{
	size_t	wi;

	if (validate_redir(op, op_i, 2) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op->redirs[op->red_cnt].type = REDIR_APP;
	op->redirs[op->red_cnt].target_fd = STDOUT_FILENO;
	wi = *op_i + 2;
	skip_spaces(op->name, &wi);
	if ((op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	op->redirs[op->red_cnt].path = (char *)malloc(PATH_MAX * sizeof(char));
	if (!op->redirs[op->red_cnt].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	if (extr_redir_path(op, &wi, op->redirs[op->red_cnt].path, PATH_MAX) == -1)
		return (COMMON_FAILURE);
	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

/* Handles heredoc '<<'. Prepares the delimiter and
 * allocates memory for the eventual content.*/
int	add_heredoc(t_operand *op, size_t *op_i)
{
	size_t	wi;

	if (validate_redir(op, op_i, 2) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	init_heredoc(op);
	wi = *op_i + 2;
	skip_spaces(op->name, &wi);
	if ((op->name[wi] == '>' || op->name[wi] == '<')
		&& !is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}
	if (get_hd_delimiter(op, &wi) != COMMON_SUCCESS)
		return (COMMON_FAILURE);
	op->redirs[op->red_cnt].hd.content = malloc(MAX_HD_CONTENT_LEN
			* sizeof(char));
	if (!op->redirs[op->red_cnt].hd.content)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}
