#include "redirect_parser.h"

#include <stdio.h>
#include <stdlib.h>

/* Preliminary heredoc parser.
 *     ti	- token index;
 *     op_i	- operand's content index;
 *     wi   - redirection's operand index
 *			  (a 'word' that goes after a
 *			  redirection or heredoc);
 *     i	- auxiliary counter */
int	redirections_parser(t_parser_data *d)
{
	t_operand	*op;
	size_t		ti;
	size_t		op_i;
	size_t		op_slen;
	int			fret;

	ti = 0;
	fret = COMMON_SUCCESS;
	while (ti < d->token_cnt)
	{
		if (d->tokens[ti].type == OPERAND)
		{
			op = d->tokens[ti].op;
			op_i = 0;
			op_slen = ft_strlen(op->name);
			while (op_i < op_slen)
			{
				// Order of condition here is important!
				// Otherwise program may think '<<' is '<'
				if (is_redir_app(op, op_i))
				{
					fret = add_redir_app(op, &op_i);
					if (fret != COMMON_SUCCESS)
						return (fret);
				}
				else if (is_heredoc(op, op_i))
				{
					fret = add_heredoc(op, &op_i);
					if (fret != COMMON_SUCCESS)
						return (fret);
				}
				else if (is_redir_out(op, op_i))
				{
					fret = add_redir_out(op, &op_i);
					if (fret != COMMON_SUCCESS)
						return (fret);
				}
				else if (is_redir_in(op, op_i))
				{
					fret = add_redir_in(op, &op_i);
					if (fret != COMMON_SUCCESS)
						return (fret);
				}
				++op_i;
			}
		}
		++ti;
	}
	return (fret);
}

int	add_redir_in(t_operand *op, size_t *op_i)
{
	size_t	wi;
	size_t	i;
	size_t	op_slen;

	// Overflow check
	if (op->red_cnt == MAX_REDIRS_NUM - 1)
	{
		print_shell_error(NULL, TOO_MANY_REDIRECTS);
		return (COMMON_FAILURE);
	}

	// At the end of operand string
	if (*op_i == ft_strlen(op->name) - 1)
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}

	op->redirs[op->red_cnt].type = REDIR_IN;
	op->redirs[op->red_cnt].target_fd = STDIN_FILENO;
	wi = *op_i + 1; // Skip <
	skip_spaces(op->name, &wi);

	// After this redirection immediately goes another one
	if ((op->name[wi] == '>' || op->name[wi] == '<') &&
		!is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}

	// Let's go coping until the end of the operand string,
	// or until we find a space outside any quotes,
	// or until we find another redirection or heredoc
	
	// Let's allocate memory for this redirection's path
	op->redirs[op->red_cnt].path = (char *)malloc(PATH_MAX * sizeof(char));
	if (!op->redirs[op->red_cnt].path)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
		
	i = 0;
	op_slen = ft_strlen(op->name);
	while (wi < op_slen)
	{
		// Overflow check
		if (i >= PATH_MAX - 1)
		{
			print_shell_error(NULL, TOO_LONG_REDIRECT_PATH);
			return (COMMON_FAILURE);
		}
		// We reach another redirection or redirection's operand has terminated
		if ((op->name[wi] == ' ' || op->name[wi] == '>' ||
			op->name[wi] == '<') && !is_inside_op_quotes(op, wi))
		{
			break ;
		}
		op->redirs[op->red_cnt].path[i++] = op->name[wi++];
	}
	op->redirs[op->red_cnt].path[i] = '\0';

	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

int	add_redir_out(t_operand *op, size_t *op_i)
{
	size_t	wi;
	size_t	i;
	size_t	op_slen;

	if (op->red_cnt == MAX_REDIRS_NUM - 1)
	{
		print_shell_error(NULL, TOO_MANY_REDIRECTS);
		return (COMMON_FAILURE);
	}
	if (*op_i == ft_strlen(op->name) - 1)
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}

	op->redirs[op->red_cnt].type = REDIR_OUT;
	op->redirs[op->red_cnt].target_fd = STDOUT_FILENO;
	wi = *op_i + 1;
	skip_spaces(op->name, &wi);

	if ((op->name[wi] == '>' || op->name[wi] == '<') &&
		!is_inside_op_quotes(op, wi))
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
	i = 0;
	op_slen = ft_strlen(op->name);
	while (wi < op_slen)
	{
		if (i >= PATH_MAX - 1)
		{
			print_shell_error(NULL, TOO_LONG_REDIRECT_PATH);
			return (COMMON_FAILURE);
		}
		if ((op->name[wi] == ' ' || op->name[wi] == '>' ||
			op->name[wi] == '<') && !is_inside_op_quotes(op, wi))
		{
			break ;
		}
		op->redirs[op->red_cnt].path[i++] = op->name[wi++];
	}
	op->redirs[op->red_cnt].path[i] = '\0';

	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

int	add_redir_app(t_operand *op, size_t *op_i)
{
	size_t	wi;
	size_t	i;
	size_t	op_slen;

	if (op->red_cnt == MAX_REDIRS_NUM - 1)
	{
		print_shell_error(NULL, TOO_MANY_REDIRECTS);
		return (COMMON_FAILURE);
	}
	// At the end of operand string
	if (*op_i == ft_strlen(op->name) - 2)
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}

	op->redirs[op->red_cnt].type = REDIR_APP;
	op->redirs[op->red_cnt].target_fd = STDOUT_FILENO;
	wi = *op_i + 2; // Skip >>
	skip_spaces(op->name, &wi);

	if ((op->name[wi] == '>' || op->name[wi] == '<') &&
		!is_inside_op_quotes(op, wi))
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
	i = 0;
	op_slen = ft_strlen(op->name);
	while (wi < op_slen)
	{
		if (i >= PATH_MAX - 1)
		{
			print_shell_error(NULL, TOO_LONG_REDIRECT_PATH);
			return (COMMON_FAILURE);
		}
		if ((op->name[wi] == ' ' || op->name[wi] == '>' ||
			op->name[wi] == '<') && !is_inside_op_quotes(op, wi))
		{
			break ;
		}
		op->redirs[op->red_cnt].path[i++] = op->name[wi++];
	}
	op->redirs[op->red_cnt].path[i] = '\0';

	*op_i = --wi;
	++op->red_cnt;
	return (COMMON_SUCCESS);
}

int	add_heredoc(t_operand *op, size_t *op_i)
{
	size_t	wi;
	size_t	i;
	size_t	op_slen;

	if (op->red_cnt == MAX_REDIRS_NUM - 1)
	{
		print_shell_error(NULL, TOO_MANY_REDIRECTS);
		return (COMMON_FAILURE);
	}
	// At the end of operand string
	if (*op_i == ft_strlen(op->name) - 2)
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}

	op->redirs[op->red_cnt].type = REDIR_HEREDOC;
	op->redirs[op->red_cnt].target_fd = STDIN_FILENO;
	op->redirs[op->red_cnt].hd.cnt_len = 0;
	wi = *op_i + 2; // Skip <<
	skip_spaces(op->name, &wi);

	if ((op->name[wi] == '>' || op->name[wi] == '<') &&
		!is_inside_op_quotes(op, wi))
	{
		print_shell_error(NULL, REDIRECT_ERR_MSG);
		return (COMMON_FAILURE);
	}

	// Now let's get the delimiter
	op->redirs[op->red_cnt].hd.delim = (char *)malloc(MAX_HD_DELIM_LEN * sizeof(char));
	if (!op->redirs[op->red_cnt].hd.delim)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	i = 0;
	op_slen = ft_strlen(op->name);
	while (wi < op_slen)
	{
		if (i >= MAX_HD_DELIM_LEN - 1)
		{
			print_shell_error(NULL, TOO_LONG_HD_DELIM);
			return (COMMON_FAILURE);
		}
		if ((op->name[wi] == ' ' || op->name[wi] == '>' ||
			op->name[wi] == '<') && !is_inside_op_quotes(op, wi))
		{
			break ;
		}
		op->redirs[op->red_cnt].hd.delim[i++] = op->name[wi++];
	}
	op->redirs[op->red_cnt].hd.delim[i] = '\0';	
	*op_i = --wi;
	// Set f_expand_body flag
	if (is_surrounded_quotes(op->redirs[op->red_cnt].hd.delim))
	{
		erase_quotes_no_check(op->redirs[op->red_cnt].hd.delim);
		op->redirs[op->red_cnt].hd.f_expand_body = false;
	}
	else
		op->redirs[op->red_cnt].hd.f_expand_body = true;

	// Allocate memory for heredoc's content
	op->redirs[op->red_cnt].hd.content = (char *)malloc(MAX_HD_CONTENT_LEN * sizeof(char));
	if (!op->redirs[op->red_cnt].hd.content)
	{
		perror("malloc");
		return (COMMON_SYS_ERR);
	}
	++op->red_cnt;
	return (COMMON_SUCCESS);
}
