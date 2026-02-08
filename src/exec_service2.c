#include "exec.h"
#include "operand.h"

static bool	token_is_redir_in(t_op_token *op_tok);
static bool	token_is_redir_out(t_op_token *op_tok);
static bool	token_is_redir_app(t_op_token *op_tok);
static bool	token_is_heredoc(t_op_token *op_tok);

bool	exec_token_is_redirect(t_op_token *op_tok)
{
	if (token_is_redir_in(op_tok) || token_is_redir_out(op_tok) ||
		token_is_redir_app(op_tok) || token_is_heredoc(op_tok))
	{
		return (true);
	}
	return (false);
}

static bool	token_is_redir_in(t_op_token *op_tok)
{
	if (op_tok->cnt[0] == '<' && op_tok->cnt[1] == '\0')
		return (true);
	return (false);
}

static bool	token_is_redir_out(t_op_token *op_tok)
{
	if (op_tok->cnt[0] == '>' && op_tok->cnt[1] == '\0')
		return (true);
	return (false);
}

static bool	token_is_redir_app(t_op_token *op_tok)
{
	if (op_tok->cnt[0] == '>' && op_tok->cnt[1] == '>' &&
		op_tok->cnt[2] == '\0')
	{
		return (true);
	}
	return (false);
}

static bool	token_is_heredoc(t_op_token *op_tok)
{
	if (op_tok->cnt[0] == '<' && op_tok->cnt[1] == '<' &&
		op_tok->cnt[2] == '\0')
	{
		return (true);
	}
	return (false);
}
