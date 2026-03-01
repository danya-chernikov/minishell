/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve8.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 02:38:26 by dchernik          #+#    #+#             */
/*   Updated: 2026/03/01 02:38:27 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "shell.h"
#include "token.h"
#include "operand.h"
#include "builtin.h"
#include "expansion.h"

#include "error.h"

#include <stdlib.h>
#include <errno.h>

static void	child_cleanup_exit(t_operand *op, char *path,
				char **envp, int status);
static void	child_run_builtin_or_exit(t_shell *msh, t_token *token);
static void	child_run_external_or_exit(t_shell *msh, t_token *token);

void	child_exec_operand(t_shell *msh, t_token *token)
{
	if (prepare_operand(msh, token) != COMMON_SUCCESS)
		exit(RET_CMD_FAILURE);
	if (apply_redirs(token->op) != COMMON_SUCCESS)
		child_cleanup_exit(token->op, NULL, NULL, RET_CMD_FAILURE);
	if (token->op->argc == 0)
		child_cleanup_exit(token->op, NULL, NULL, RET_CMD_SUCCESS);
	if (is_any_builtin(token->op->argv[0]))
		child_run_builtin_or_exit(msh, token);
	child_run_external_or_exit(msh, token);
}

static void	child_run_builtin_or_exit(t_shell *msh, t_token *token)
{
	int	status;

	if (token->op->f_per_cmd)
		env_apply_as_env(&msh->env, token->op->my_env);
	status = run_builtin(msh, token->op, BUILTIN_IN_CHILD);
	child_cleanup_exit(token->op, NULL, NULL, status);
}

static void	child_run_external_or_exit(t_shell *msh, t_token *token)
{
	char	*path;
	char	**envp;
	int		status;

	envp = build_envp_for_operand(msh, token->op);
	if (!envp)
		child_cleanup_exit(token->op, NULL, NULL, RET_CMD_FAILURE);
	path = resolve_cmd_path(msh, token->op->argv[0]);
	if (!path)
	{
		print_shell_error(token->op->argv[0], CMD_NOT_FOUND_ERR_MSG);
		child_cleanup_exit(token->op, NULL, envp, CMD_NOT_LOCATED_ERR);
	}
	execve(path, token->op->argv, envp);
	status = map_exec_errno(errno);
	print_sys_error(token->op->argv[0]);
	child_cleanup_exit(token->op, path, envp, status);
}

static void	child_cleanup_exit(t_operand *op, char *path,
				char **envp, int status)
{
	if (path)
		free(path);
	if (envp)
		free_envp(envp);
	exp_free_argv(op);
	exit(status);
}
