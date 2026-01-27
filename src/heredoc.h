#ifndef HEREDOC_H
# define HEREDOC_H

# include <sys/types.h>
# include <stdbool.h>

/* MAX_HEREDOC_NUM	- Maxinum number of heredocs
 *					  in one operand;
 * MAX_HEREDOCS_NUM	- Equals to MAX_REDIRS_NUM;
 * MAX_HD_DELIM_LEN - The same as PATH_MAX */
# define MAX_HEREDOCS_NUM	256
# define MAX_HD_DELIM_LEN	4096 
# define MAX_HD_CONTENT_LEN	8192
# define HD_READ			0
# define HD_WRITE			1

typedef struct s_parser_data	t_parser_data;
typedef struct s_shell			t_shell;

/* Describes heredoc.
 *     cnt_len - current content length */
typedef struct s_heredoc
{
	char	*content;
	char	*delim;
	size_t	cnt_len;
	bool	f_expand_body;

}	t_heredoc;

int		read_heredocs(t_shell *msh);
int		read_one_heredoc(t_shell *msh, t_heredoc *hd);
void	heredoc_child_loop(t_shell *msh, int wfd, const t_heredoc *hd);
int		heredoc_parent_collect(int rfd, t_heredoc *hd);

#endif
