#include "exec.h"
#include "env.h"
#include "token.h"
#include "operand.h"
#include "builtin.h"
#include "expansion.h"
#include "aux_common.h"

#include "error.h"

#include <stdio.h>

int		parent_run_with_redirs(t_shell *msh, t_token *token);
int		save_stdio(int *save_in, int *save_out);
void	restore_stdio(int save_in, int save_out);

int	parent_run_with_redirs(t_shell *msh, t_token *token)
{
	int	fret;
	int	save_in;
	int	save_out;
	int	status;

	status = RET_CMD_SUCCESS;
	if (save_stdio(&save_in, &save_out) != COMMON_SUCCESS)
		return (RET_CMD_FAILURE);
	fret = apply_redirs(token->op);
	if (fret != COMMON_SUCCESS)
	{
		if (fret == COMMON_SYS_ERR)
			print_shell_error(NULL, "redirect");
		restore_stdio(save_in, save_out);
		return (RET_CMD_FAILURE);
	}
	if (token->op->argc > 0)
		status = run_builtin(msh, token->op, BUILTIN_IN_PARENT);
	restore_stdio(save_in, save_out);
	return (status);
}

int	save_stdio(int *save_in, int *save_out)
{
	*save_in = dup(STDIN_FILENO);
	if (*save_in == -1)
	{
		perror("dup");
		return (COMMON_FAILURE);
	}
	*save_out = dup(STDOUT_FILENO);
	if (*save_out == -1)
	{
		perror("dup");
		close(*save_in);
		return (COMMON_FAILURE);
	}
	return (COMMON_SUCCESS);
}

void	restore_stdio(int save_in, int save_out)
{
	if (save_in != -1)
	{
		dup2(save_in, STDIN_FILENO);
		close(save_in);
	}
	if (save_out != -1)
	{
		dup2(save_out, STDOUT_FILENO);
		close(save_out);
	}
}

int	prepare_operand(t_shell *msh, t_token *token)
{
	t_operand	*op;
	int			fres;

	if (!token || token->type != OPERAND || !token->op)
		return (COMMON_FAILURE);
	op = token->op;
	exp_free_argv(op);
	exp_free_op_tokens(op);
	op->f_per_cmd = false;
	if (op->my_env)
	{
		env_free(op->my_env);
		fres = env_init(op->my_env, NULL);
		if (fres != COMMON_SUCCESS)
			return (fres);
	}
	else
	{
		fres = op_env_init(op);
		if (fres != COMMON_SUCCESS)
			return (fres);
	}
	fres = exp_alloc_argv(token->op);
	if (fres != COMMON_SUCCESS)
		return (fres);
	fres = do_all_expansions_assignments(msh, token);
	if (fres != COMMON_SUCCESS)
	{
		exp_free_argv(token->op);
		exp_free_op_tokens(op);
		op->f_per_cmd = false;
		if (op->my_env)
		{
			env_free(op->my_env);
			op->my_env = NULL;
			env_init(op->my_env, NULL);
		}
		return (fres);
	}
	return (COMMON_SUCCESS);
}
