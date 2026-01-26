#ifndef HEREDOC_H
# define HEREDOC_H

# include <sys/types.h>
# include <stdbool.h>

/*
 * MAX_HEREDOC_NUM - Maxinum number of heredocs
 *					 in one operand;
 * */
# define MAX_HEREDOCS_NUM	256
# define MAX_HD_DELIM_LEN	64
# define MAX_HD_CONTENT_LEN	8192

typedef struct s_heredoc
{
	char	*content;
	char	*delim;
	size_t	len;
	bool	f_expand_body;
	int		target_fd;		// Always 0

}	t_heredoc;

#endif
