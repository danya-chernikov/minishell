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

static void		free_envp_partial(char **envp, size_t n);
static int		copy_operand_env(t_operand *op, char **envp, size_t *ei);
static int		copy_shell_env(t_shell *msh, char **envp, size_t *ei);
static size_t	count_operand_envp(t_env *env);
static size_t	count_shell_envp(t_shell *msh, t_env *op_env);

/* We do NOT have to call msh_free() here on exit,
 * because the kernel will release memory upon
 * completion of the child process anyway */
void	child_exec_operand(t_shell *msh, t_token *token)
{
	char	*path;
	char	**envp;
	int		status;

	if (prepare_operand(msh, token) != COMMON_SUCCESS)
		exit(RET_CMD_FAILURE);
	if (apply_redirs(token->op) != COMMON_SUCCESS)
	{
		exp_free_argv(token->op);
		exit(RET_CMD_FAILURE);
	}
	if (token->op->argc == 0)
	{
		exp_free_argv(token->op);
		exit(RET_CMD_SUCCESS);
	}
	if (is_any_builtin(token->op->argv[0]))
	{
		if (token->op->f_per_cmd)
			env_apply_as_env(&msh->env, token->op->my_env);
		status = run_builtin(msh, token->op, BUILTIN_IN_CHILD);
		exp_free_argv(token->op);
		exit(status);
	}
	envp = build_envp_for_operand(msh, token->op);
	path = resolve_cmd_path(msh, token->op->argv[0]);
	if (!path)
	{
		print_shell_error(token->op->argv[0], CMD_NOT_FOUND_ERR_MSG);
		free_envp(envp);
		exp_free_argv(token->op);
		exit(CMD_NOT_LOCATED_ERR);
	}
	execve(path, token->op->argv, envp);
	status = map_exec_errno(errno);
	print_sys_error(token->op->argv[0]);
	free(path);
	free_envp(envp);
	exp_free_argv(token->op);
	exit(status);
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
	envp = malloc((total + 1) * sizeof *envp);
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

static void	free_envp_partial(char **envp, size_t n)
{
	while (n > 0)
	{
		--n;
		free(envp[n]);
	}
	free(envp);
}

static int	copy_operand_env(t_operand *op, char **envp, size_t *ei)
{
	size_t	mi;

	mi = 0;
	while (op->my_env && mi < op->my_env->vars_num)
	{
		if (op->my_env->vars[mi].name && op->my_env->vars[mi].value &&
			op->my_env->vars[mi].type != PARAM)
		{
			envp[*ei] = key_value_to_str(op->my_env->vars[mi].name,
					op->my_env->vars[mi].value);
			if (!envp[*ei])
				return (COMMON_SYS_ERR);
			++(*ei);
			envp[*ei] = NULL;
		}
		++mi;
	}
	return (COMMON_SUCCESS);
}

static int	copy_shell_env(t_shell *msh, char **envp, size_t *ei)
{
	size_t	mi;

	mi = 0;
	while (mi < msh->env.vars_num)
	{
		if (msh->env.vars[mi].name && msh->env.vars[mi].type == ENV &&
			!envp_has_name(envp, msh->env.vars[mi].name))
		{
			envp[*ei] = key_value_to_str(msh->env.vars[mi].name,
					msh->env.vars[mi].value);
			if (!envp[*ei])
				return (COMMON_SYS_ERR);
			++(*ei);
			envp[*ei] = NULL;
		}
		++mi;
	}
	return (COMMON_SUCCESS);
}

static size_t	count_operand_envp(t_env *env)
{
	size_t	i;
	size_t	cnt;

	if (!env)
		return (0);
	i = 0;
	cnt = 0;
	while (i < env->vars_num)
	{
		if (env->vars[i].name && env->vars[i].value &&
			env->vars[i].type != PARAM)
			++cnt;
		++i;
	}
	return (cnt);
}

static size_t	count_shell_envp(t_shell *msh, t_env *op_env)
{
	size_t	i;
	size_t	cnt;

	i = 0;
	cnt = 0;
	while (i < msh->env.vars_num)
	{
		if (msh->env.vars[i].name &&
			msh->env.vars[i].type == ENV &&
			(!op_env || !env_exist(op_env, msh->env.vars[i].name)))
			++cnt;
		++i;
	}
	return (cnt);
}

char	*key_value_to_str(const char *key, const char *value)
{
	size_t	key_len;
	size_t	val_len;
	size_t	total_len;
	char	*str;
	
	key_len = ft_strlen(key);
	val_len = 0;
	if (value)
		val_len = ft_strlen(value);
	total_len = key_len + val_len + 2;
	str = (char *)malloc(total_len * sizeof (char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, key, total_len);
	ft_strlcat(str, "=", total_len);
	if (value)
		ft_strlcat(str, value, total_len);
	return (str);
}
