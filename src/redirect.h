#ifndef REDIRECT_H
# define REDIRECT_H

# include "heredoc.h"

# define MAX_REDIRS_NUM	256

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
	union
	{
		char		*path;
		t_heredoc	hd;

	}	u_source;

}	t_redir;

#endif
