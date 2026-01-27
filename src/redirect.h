#ifndef REDIRECT_H
# define REDIRECT_H

# include "heredoc.h"

# define MAX_REDIRS_NUM		256
# define MAX_REDIR_PATH_LEN	4096 // The same as PATH_MAX
# define TOKEN_REDIR_IN		"<"
# define TOKEN_REDIR_OUT	">"
# define TOKEN_REDIR_APP	">>"
# define TOKEN_HEREDOC		"<<"

typedef struct s_operand	t_operand;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APP,
	REDIR_HEREDOC

}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	int				target_fd; // 0 for < and <<, and 1 for >> and >>
	char			*path;
	t_heredoc		hd;

}	t_redir;

bool	is_redir_in(t_operand *op, size_t op_i);
bool	is_redir_out(t_operand *op, size_t op_i);
bool	is_redir_app(t_operand *op, size_t op_i);
bool	is_heredoc(t_operand *op, size_t op_i);

#endif
