#include "debug.h"

void	dbg_print_configs(t_configs *cnf, t_conf_type ctype)
{
	int	i;

	printf("\nConfiguration files:\n");
	i = 0;
	if (ctype == LOGIN_CONF)
	{
		while (i < LOGIN_CONFIGS_NUM)
		{
			printf("%s\n", cnf->login[i]);
			++i;
		}
	}
	else
	{
		while (i < NONLOGIN_CONFIGS_NUM)
		{
			printf("%s\n", cnf->nonlogin[i]);
			++i;
		}
	}
	printf("\n");
}

void	dbg_print_filehistory(t_history *history)
{
	size_t	i;

	printf("\nHistory from file:\n");
	i = 0;
	while (i < history->lines_num)
	{
		printf("%s\n", history->lines[i].cmd);
		++i;
	}
	printf("\n");
}

void	dbg_prompt_parser_print_all(t_parser_data *d)
{
	dbg_prompt_parser_print_parsed_data(d);
	dbg_prompt_parser_print_tokens(d);
	dbg_prompt_parser_print_parentheses(d);
	dbg_prompt_parser_print_quotes(d);
}

void	dbg_prompt_parser_print_quotes(t_parser_data *d)
{
	size_t	i;
	char	quote;

	i = 0;
	printf("\nQuote intervals:\n");
	if (d->qpair_cnt == 0)
	{
		printf("----------\n\n");
		return ;
	}

	while (i < d->qpair_cnt)
	{
		if (d->quotes[i].type == DOUBLE_QUOTE)
			quote = '"';
		else
			quote = '\'';
		printf("%zu\t%c\t%zu\t%zu\n",
			i + 1,
			quote,
			d->quotes[i].li,
			d->quotes[i].ri);
		++i;
	}
	printf("\n");
}

void	dbg_prompt_parser_print_parsed_data(t_parser_data *d)
{
	size_t	i;

	// Let's output the pipes we found
	i = 0; 
	printf("\nPipes:\n");
	if (d->pipe_cnt == 0)
	{
		printf("----------\n\n");
		return ;
	}
	while (i < d->pipe_cnt)
	{
		printf("%lu: [%d] [%d]\n", i + 1,
			d->pipes[i][READ_END], d->pipes[i][WRITE_END]);
		++i;
	}
	printf("\n");

	// Let's output the operands we found
	i = 0; 
	printf("\nOperands:\n");
	if (d->op_cnt == 0)
	{
		printf("----------\n\n");
		return ;
	}
	while (i < d->op_cnt)
	{
		printf("%lu: [%s] [%d] [%d]\n", i + 1,
			d->ops[i].name, d->ops[i].read_end, d->ops[i].write_end);
		++i;
	}
	printf("\n");
}

void	dbg_prompt_parser_print_tokens(t_parser_data *d)
{
	char	format[MAX_FORMAT_STR_LEN];
	size_t	i;

	strncpy(format, "%d\t%s\t%lu\n", MAX_FORMAT_STR_LEN);
	i = 0;
	printf("\nTokens:\n");
	if (d->token_cnt == 0)
	{
		printf("----------\n\n");
		return ;
	}
	while (i < d->token_cnt)
	{
		if (d->tokens[i].type == OPERAND)
			printf(format, i, d->tokens[i].op->name, d->tokens[i].start_pi);
		else if (d->tokens[i].type == PIPE)
			printf(format, i, TOKEN_PIPE, d->tokens[i].start_pi);
		else if (d->tokens[i].type == OPEN_PAR)
			printf(format, i, TOKEN_OPEN_PAR, d->tokens[i].start_pi);
		else if (d->tokens[i].type == CLOSE_PAR)
			printf(format, i, TOKEN_CLOSE_PAR, d->tokens[i].start_pi);
		else if (d->tokens[i].type == AND)
			printf(format, i, TOKEN_AND, d->tokens[i].start_pi);
		else if (d->tokens[i].type == OR)
			printf(format, i, TOKEN_OR, d->tokens[i].start_pi);
		++i;
	}
	printf("\n");
}

void	dbg_prompt_parser_print_parentheses(t_parser_data *d)
{	
	size_t	i;

	i = 0;
	printf("\nParentheses:\n");
	if (d->par_cnt == 0)
		printf("----------\n\n");
	else
	{
		printf("#\t(\t)\n");
		while (i < d->par_cnt)
		{
			printf("%lu\t%lld\t%lld\n", i + 1, d->pars[i].first, d->pars[i].second);
			++i;
		}
		printf("\n");
	}
}

void	dbg_print_redirs(t_parser_data *d)
{
	t_redir	*r;
	size_t	op_i;
	size_t	ri;

	printf("\nRedirections:\n");
	op_i = 0;
	if (d->op_cnt == 0)
	{
		printf("----------\n\n");
		return ;
	}
	while (op_i < d->op_cnt)
	{
		printf("%zu. %s\n", op_i, d->ops[op_i].name);

		if (d->ops[op_i].red_cnt == 0)
		{
			printf("----------\n");
		}
		else
		{
			ri = 0;
			while (ri < d->ops[op_i].red_cnt)
			{
				r = &d->ops[op_i].redirs[ri];

				printf("\t%zu\n", ri);

				if (r->type == REDIR_IN)
					printf("\tREDIR_IN\n");
				else if (r->type == REDIR_OUT)
					printf("\tREDIR_OUT\n");
				else if (r->type == REDIR_APP)
					printf("\tREDIR_APP\n");
				else if (r->type == REDIR_HEREDOC)
					printf("\tREDIR_HEREDOC\n");

				printf("\tTarget FD: %d\n", r->target_fd);

				if (r->type == REDIR_HEREDOC)
				{
					if (r->hd.content)
						printf("\tContent: %s\n", r->hd.content);
					if (r->hd.delim)
						printf("\tDelimiter: %s\n", r->hd.delim);
					if (r->hd.f_expand_body)
						printf("\tExpand body?: YES\n");
					else	
						printf("\tExpand body?: NO\n");
				}
				else
				{
					printf("\tOperand-path: %s\n", r->path);
				}

				++ri;
			}
		}
		++op_i;
	}
	printf("\n");
}
