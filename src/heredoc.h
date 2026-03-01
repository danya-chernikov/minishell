/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:02:30 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 03:02:31 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include <sys/types.h>
# include <stdbool.h>
# include <termios.h>
# include <signal.h>

/* MAX_HEREDOC_NUM	- Maxinum number of heredocs
 *					  in one operand;
 * MAX_HEREDOCS_NUM	- Equals to MAX_REDIRS_NUM;
 * MAX_HD_DELIM_LEN - The same as PATH_MAX */
# define MAX_HEREDOCS_NUM	256
# define MAX_HD_DELIM_LEN	4096 
# define MAX_HD_CONTENT_LEN	8192
# define HD_READ			0
# define HD_WRITE			1
# define HD_HAVE_TTY		0
# define HD_RESTORE			1

typedef struct s_parser_data	t_parser_data;
typedef struct s_operand		t_operand;
typedef struct s_shell			t_shell;
typedef struct s_vector			t_vector;

/* To pass the Norm */
typedef struct s_hd_helper
{
	pid_t				pid;
	bool				flags[2];
	struct sigaction	old[2];
}	t_hd_helper;

/* Describes heredoc.
 *     cnt_len - current content length */
typedef struct s_heredoc
{
	char	*content;
	char	*delim;
	size_t	cnt_len;
	bool	f_expand_body;
}	t_heredoc;

/* heredoc.c */
int		read_heredocs(t_shell *msh);
int		read_one_operand_hd(t_shell *msh, t_operand *op);
int		read_one_heredoc(t_shell *msh, t_heredoc *hd);
int		hd_prepare_parent(int p[2], bool flags[2],
			struct termios *saved, struct sigaction old[2]);
int		hd_fork_failed(int p[2], bool flags[2],
			struct termios *saved, struct sigaction old[2]);

/* heredoc2.c */
void	hd_restore_parent(bool flags[2], struct termios *saved,
			struct sigaction old[2]);
int		hd_wait_child(pid_t pid, int *status);
int		hd_finish_status(int status, int ret_code);
void	heredoc_child_loop(t_shell *msh, int wfd, const t_heredoc *hd);
int		hd_handle_line(t_shell *msh, int wfd,
			const t_heredoc *hd, char *line);

/* heredoc3.c */
int		hd_write_line(t_shell *msh, int wfd,
			const t_heredoc *hd, char *line);
int		hd_write_out(int wfd, const char *out);
int		heredoc_parent_collect(int rfd, t_heredoc *hd);
int		hd_append_buf(t_heredoc *hd, char *buf, ssize_t rlen);
void	hd_copy_buf(t_heredoc *hd, char *buf, size_t len);

/* heredoc4.c */
char	*heredoc_expand_line(t_shell *msh, const char *line);
void	hd_push_dollar(t_shell *msh, t_vector *v,
			const char *line, size_t *i);
void	hd_push_value(t_shell *msh, t_vector *v, char *name);
void	hd_read_name(const char *line, char *name, size_t *i);
char	*hd_finish_expand(t_vector *v);

#endif
