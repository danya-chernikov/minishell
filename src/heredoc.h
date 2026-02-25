/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:51:27 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 23:41:28 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_hd_ctx
{
	int					status;
	int					jret_code;
	bool				f_have_tty;
	struct termios		saved;
	struct sigaction	old_int;
	struct sigaction	old_quit;
}	t_hd_ctx;

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
void	hd_init_ctx(t_hd_ctx *ctx);
int		hd_finish(t_hd_ctx *ctx);
void	hd_expand_var(t_shell *msh, t_vector *out, const char *line, size_t *i);
void	hd_expand_special(t_shell *msh, t_vector *out,
			const char *line, size_t *i);

/* heredoc2.c */
void	hd_expand_dollar(t_shell *msh, t_vector *out,
			const char *line, size_t *i);
int		hd_write_raw(int wfd, char *line);
char	*heredoc_expand_line(t_shell *msh, const char *line);
void	push_expanded_var(t_shell *msh, t_vector *v, const char *line,
			size_t *i);

/* heredoc3.c */
int		read_one_heredoc(t_shell *msh, t_heredoc *hd);
int		hd_fork_error(t_hd_ctx *ctx, int p[2]);
int		heredoc_parent_collect(int rfd, t_heredoc *hd);
int		hd_collect_chunk(t_heredoc *hd, const char *buf, ssize_t rlen);

/* heredoc4.c */
void	heredoc_child_loop(t_shell *msh, int wfd, const t_heredoc *hd);
char	*hd_read_line(t_shell *msh, int wfd);
int		hd_write_expanded(t_shell *msh, int wfd, char *line);
void	hd_build_vector(t_shell *msh, t_vector *out, const char *line);

#endif
