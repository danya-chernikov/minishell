/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhvalenc <jhvalenc@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:47:11 by jhvalenc          #+#    #+#             */
/*   Updated: 2026/02/26 00:28:09 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "shell.h"
#include "token.h"
#include "operand.h"
#include "builtin.h"
#include "expansion.h"
#include "aux_common.h"

#include "error.h"
#include "libft.h"

#include <stdlib.h>

/* We do NOT have to call msh_free() here on exit,
 * because the kernel will release memory upon
 * completion of the child process anyway */
void	child_exec_operand(t_shell *msh, t_token *token)
{
	if (prepare_operand(msh, token) != COMMON_SUCCESS)
		exit(RET_CMD_FAILURE);
	if (apply_redirs(token->op) != COMMON_SUCCESS
		|| token->op->argc == 0)
		child_cleanup_exit(token, NULL, NULL,
			(token->op->argc != 0));
	if (is_any_builtin(token->op->argv[0]))
		child_exec_builtin(msh, token);
	child_exec_external(msh, token);
}

void	child_exec_builtin(t_shell *msh, t_token *token)
{
	int	status;

	if (token->op->f_per_cmd)
		env_apply_as_env(&msh->env, token->op->my_env);
	status = run_builtin(msh, token->op, BUILTIN_IN_CHILD);
	child_cleanup_exit(token, NULL, NULL, status);
}

void	child_exec_external(t_shell *msh, t_token *token)
{
	char	*path;
	char	**envp;
	int		status;

	envp = build_envp_for_operand(msh, token->op);
	path = resolve_cmd_path(msh, token->op->argv[0]);
	if (!path)
	{
		print_shell_error(token->op->argv[0],
			CMD_NOT_FOUND_ERR_MSG);
		child_cleanup_exit(token, envp, NULL,
			CMD_NOT_LOCATED_ERR);
	}
	execve(path, token->op->argv, envp);
	status = map_exec_errno(errno);
	print_sys_error(token->op->argv[0]);
	child_cleanup_exit(token, envp, path, status);
}

/*  Cleans up operand data and environment strings before exiting.*/
void	child_cleanup_exit(t_token *token, char **envp, char *path,
		int stat)
{
	if (path)
		free(path);
	if (envp)
		free_envp(envp);
	if (token && token->op)
		exp_free_argv(token->op);
	exit(stat);
}

char	**build_envp_for_operand(t_shell *msh, t_operand *op)
{
	int		fret;
	size_t	ei;
	size_t	total;
	char	**envp;

	total = 0;
	total += count_operand_envp(op->my_env);
	total += count_shell_envp(msh, op->my_env);
	envp = malloc((total + 1) * sizeof (*envp));
	if (!envp)
		return (NULL);
	ei = 0;
	envp[0] = NULL;
	fret = copy_operand_env(op, envp, &ei);
	if (fret != COMMON_SUCCESS)
		return (free_envp_partial(envp, ei), NULL);
	fret = copy_shell_env(msh, envp, &ei);
	if (fret != COMMON_SUCCESS)
		return (free_envp_partial(envp, ei), NULL);
	return (envp);
}
