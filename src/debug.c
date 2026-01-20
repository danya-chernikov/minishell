#include "debug.h"

void	dbg_print_configs(t_configs *cnf, t_conf_type ctype)
{
	int	i;

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
}

void	print_all(t_parser_data *d)
{
	print_parsed_data(d);
	print_tokens(d);
	print_parentheses(d);
	print_quotes(d);
}

void	print_quotes(t_parser_data *d)
{
	size_t	i;
	char	quote;

	i = 0;
	printf("\nQuote intervals:\n");
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
}

void	print_parsed_data(t_parser_data *d)
{
	size_t	i;

	// Let's output the pipes we found
	i = 0; 
	printf("\nPipes:\n");
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
	while (i < d->op_cnt)
	{
		printf("%lu: [%s] [%d] [%d]\n", i + 1,
			d->ops[i].name, d->ops[i].read_end, d->ops[i].write_end);
		++i;
	}
	printf("\n");
}

void	print_tokens(t_parser_data *d)
{
	char	format[MAX_FORMAT_STR_LEN];
	size_t	i;

	strncpy(format, "%d\t%s\t%lu\n", MAX_FORMAT_STR_LEN);
	i = 0;
	printf("\nTokens:\n");
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

void	print_parentheses(t_parser_data *d)
{	
	size_t	i;

	i = 0;
	printf("\nParentheses:\n");
	if (d->par_cnt == 0)
	{
		printf("----------\n\n");
	}
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
