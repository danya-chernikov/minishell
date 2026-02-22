#include "builtin.h"
#include "shell.h"
#include "operand.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static int	parse_export_assignment(char *s, char **var_name, char **var_value);
static int	builtin_export_alg(t_shell *msh, char *var_name, char *var_value);

int	builtin_export(t_shell *msh, t_operand *op)
{
	int		fret;
	int		arg_i;
	char	*var_name;
	char	*var_value;

	arg_i = 1;
	while (arg_i < op->argc)
	{
		var_name = NULL;
		var_value = NULL;
		fret = parse_export_assignment(op->argv[arg_i], &var_name, &var_value);
		if (fret != COMMON_SUCCESS)
			return (RET_CMD_FAILURE);
		fret = builtin_export_alg(msh, var_name, var_value);
		if (fret != COMMON_SUCCESS)
		{
			if (fret == COMMON_SYS_ERR)
				perror("malloc");
			return (RET_CMD_FAILURE);
		}
		++arg_i;
	}
	return (RET_CMD_SUCCESS);
}

static int	builtin_export_alg(t_shell *msh, char *var_name, char *var_value)
{
	int	fret;

	if (var_value)
	{
		fret = env_set(&msh->env, var_name, var_value, ENV);
		if (fret != COMMON_SUCCESS)
			return (fret);
	}
	if (env_exist(&msh->env, var_name))
	{
		env_export(&msh->env, var_name);
		free(var_name);
		return (COMMON_SUCCESS);
	}
	fret = env_set(&msh->env, var_name, ft_strdup(""), ENV);
	return (fret);
}

/* Parses things like 'export VAR=VALUE' */
static int	parse_export_assignment(char *s, char **var_name, char **var_value)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '=')
		++i;
	if (!s[i])
	{
		*var_name = ft_strdup(s);
		*var_value = NULL;
		if (*var_name)
			return (COMMON_SUCCESS);
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	}
	*var_name = ft_substr(s, 0, i);
	if (!(*var_name))
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	*var_value = ft_strdup(s + i + 1);
	if (!(*var_value))
	{
		free(*var_name);
		*var_name = NULL;
		return (perror_and_return("malloc", COMMON_SYS_ERR));
	}
	return (COMMON_SUCCESS);
}
