/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:50:39 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 23:52:03 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <stdbool.h>
# include <stddef.h>
# include <signal.h>

/* MAX_SUBSHS_NUM	- Maximum number of subshells;
 * MAX_DEPTH		- kinda the same as MAX_SUBSHS_NUM */
# define MAX_SUBSHS_NUM			64
# define MAX_DEPTH				64
# define NOT_EXEC_IN_PARENT		-2
# define CLOSE_PAR_NOT_FOUND	-1
# define SIG_OLD_INT			0
# define SIG_OLD_QUIT			1
/* We're gonna use only MAX_DEPTH */

typedef struct s_shell			t_shell;
typedef struct s_token			t_token;
typedef struct s_operand		t_operand;
typedef struct s_redir			t_redir;
typedef struct s_parser_data	t_parser_data;

typedef struct s_range
{
	size_t	l;
	size_t	r;
}	t_range;

/* st	- stage
 * nst	- number of stages */
typedef struct s_pipeline
{
	t_range	*stages;
	pid_t	*pids;
	int		(*pipes)[2];
	int		stages_num;
}	t_pipeline;

/* exec_expassign.c */
int		do_all_expansions_assignments(t_shell *msh, t_token *token);
int		do_expansions_assignments_2nd_lvl_token(t_shell *msh, t_operand *op,
			size_t *opt_i, int *redir_cnt);

/* exec_ops.c */
int		exec_ops(t_shell *msh, int *ret_code);
int		exec_ops_range(t_shell *msh, size_t l, size_t r, int depth);
int		exec_range_loop(t_shell *msh, size_t i, size_t r, int depth);
int		find_term_stop(t_parser_data *pd, size_t i, size_t r);
size_t	next_i_after_logic(t_parser_data *pd, int last, size_t stop, size_t r);

/* exec_ops2.c */
int		skip_term(t_parser_data *pd, size_t s, size_t r);
int		check_depth_limit(t_parser_data *pd, size_t l, size_t r, int base);

/* exec_pipeline.c */
int		pl_exec_pipeline(t_shell *msh, size_t l, size_t r, int depth);
int		pl_fill_stages(t_parser_data *pd, size_t l, size_t r, t_pipeline *pl);

/* exec_pipeline2.c */
int		pl_fork_one_stage(t_shell *msh, t_pipeline *pl, int st_i, int depth);
void	pl_child_run_subshell(t_shell *msh, t_range stage, int depth);
int		pl_count_stages(t_parser_data *pd, size_t l, size_t r);
int		pl_find_close(t_parser_data *pd, size_t open_i, size_t r);
int		pl_make_pipes(t_pipeline *pl);

/* exec_pipeline3.c */
int		pl_close_all_pipes(t_pipeline *pl);
int		pl_alloc(t_pipeline *pl);
void	pl_free(t_pipeline *pl);
int		pl_wait(t_pipeline *pl);
int		pl_wait_status(int ws);

/* exec_pipeline4.c */
int		pl_parent_single_try(t_shell *msh, t_token *token, int depth);
int		pl_spawn_all(t_shell *msh, t_pipeline *pl, int depth);

/* exec_execve.c */
int		parent_run_with_redirs(t_shell *msh, t_token *token);
int		save_stdio(int *save_in, int *save_out);
void	restore_stdio(int save_in, int save_out);
int		prepare_operand(t_shell *msh, t_token *t);

/* exec_execve2.c */
void	child_exec_operand(t_shell *msh, t_token *token);
void	child_exec_builtin(t_shell *msh, t_token *token);
void	child_exec_external(t_shell *msh, t_token *token);
void	child_cleanup_exit(t_token *token, char **envp, char *path,
		int stat);
char	**build_envp_for_operand(t_shell *msh, t_operand *op);

/* exec_execve3.c */
int		map_exec_errno(int errnum);
char	*resolve_cmd_path(t_shell *msh, char *cmd);
char	*resolve_in_path(t_shell *msh, const char *cmd);
char	*check_next_path(const char *path, size_t *i, const char *cmd);
char	*try_path_dir(const char *dir, const char *cmd);

/* exec_execve4.c */
int		apply_redirs(t_operand *op);
int		redir_open_fd(t_redir *redir, int *out_fd);
int		open_heredoc(t_redir *r, int *out_fd);
char	*key_value_to_str(const char *key, const char *value);

/* exec_execve5.c */
bool	has_slash(const char *s);
char	*get_full_path_from_cwd(const char *rel_path);
bool	envp_has_name(char **envp, const char *name);
bool	envp_name_eq(const char *envs, const char *name);
void	free_envp(char **envp);

/* exec_execve6.c */
bool	is_parent_builtin(const char *s);
bool	is_any_builtin(const char *s);
void	child_set_default_signals(void);

/* exec_execve7.c */
void	free_envp_partial(char **envp, size_t n);
int		copy_operand_env(t_operand *op, char **envp, size_t *ei);
int		copy_shell_env(t_shell *msh, char **envp, size_t *ei);
size_t	count_operand_envp(t_env *env);
size_t	count_shell_envp(t_shell *msh, t_env *op_env);


#endif
