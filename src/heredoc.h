#ifndef HEREDOC_H
# define HEREDOC_H

# include <sys/types.h>
# include <stdbool.h>

/*
 * MAX_HEREDOC_NUM - Maxinum number of heredocs
 *					 in one operand;
 * */
# define MAX_HEREDOCS_NUM	256 // Equal to MAX_REDIRS_NUM
# define MAX_HD_DELIM_LEN	4096 // The same as PATH_MAX
# define MAX_HD_CONTENT_LEN	8192

typedef struct s_heredoc
{
	char	*content;
	char	*delim;
	size_t	cnt_len;
	bool	f_expand_body;

}	t_heredoc;

#endif
