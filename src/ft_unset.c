#include "builtin.h"

static int	is_valid_identifier(char *str)
{
	int	index;

	index = 0;
	if (str == NULL || str[0] == '\0' || ft_isdigit(str[0]))
		return (0);
	while (str[index] != '\0')
	{
		if (ft_isalnum(str[index]) == 0 && str[index] != '_')
			return (0);
		index++;
	}
	return (1);
}

int	ft_unset(char **args, t_env *env)
{
	int	i;
	int	status;
	char	*error_unset;
	char	*error_id;

	status = 0;
	i = 1;
	while (args[i] != NULL)
	{
		if (is_valid_identifier(args[i]) == 0)
		{
			error_unset = "minishell: unset: `";
			write(STDERR_FILENO, error_unset, ft_strlen(error_unset));
			write(STDERR_FILENO, args[i], ft_strlen(args[i]));
			error_id = "': not a valid identifier\n";
			write(STDERR_FILENO, error_id, ft_strlen(error_id));
			status = 1;
		}
		else
		{
			// Si no existe, no hace nada (bash no da error)
			env_unset(env, args[i]);
		}
		i++;
	}
	return (status);
}
