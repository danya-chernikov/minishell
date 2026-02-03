#include "builtin.h"

#if 0
Verifica si el nombre de la variable es válido.
(Alfanumérico o '_', no puede empezar por número)
#endif
static int	is_valid_identifier(char *str)
{
	int	index;

	index = 0;
	if (str == NULL || str[0] == '\0' || ft_isdigit(str[0]))
		return (0);
	while (str[index] != '\0' && str[index] != '=')
	{
		if (ft_isalnum(str[index]) == 0 && str[index] != '_')
			return (0);
		index++;
	}
	return (1);
}

static int	ft_strcmp(const char *str1, const char *str2)
{
	while (*str1 != '\0' || *str2 != '\0')
	{
		if (*str1 != *str2)
			return ((unsigned char)*str1 - (unsigned char)*str2);
		str1++;
		str2++;
	}
	return (0);
}

#if 0
Imprime las variables en formato 'declare -x NOMBRE="VALOR"'.
Las ordenamos temporalmente para imitar a Bash.
#endif
static void	print_sorted_export(t_env *env)
{
	t_env_var	*sorted[MAX_TOTAL_VARS_NUM];
	t_env_var	*temp;
	size_t		i;
	size_t		j;

	i = 0;
	while (i < env->vars_num)
	{
		sorted[i] = &env->vars[i];
		i++;
	}
	i = 0;
	while (i < env->vars_num)
	{
		j = 0;
		while (j < env->vars_num - 1 - i)
		{
			if (ft_strcmp(sorted[j]->name, sorted[j + 1]->name) > 0)
			{
				temp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	// 3. Imprimir
	i = 0;
	while (i < env->vars_num)
	{
		if (sorted[i]->name && sorted[i]->type == ENV)
		{
			printf("declare -x %s", sorted[i]->name);
			if (sorted[i]->value != NULL)
				printf("=\"%s\"", sorted[i]->value);
			printf("\n");
		}
		i++;
	}
}

int	ft_export(char **args, t_env *env)
{
	int	i;
	int	status;
	char	*eq_pos;
	char	*name;
	char	*error_export;
	char	*error_id;

	status = 0;
	if (args[1] == NULL)
	{
		print_sorted_export(env);
		return (0);
	}
	i = 1;
	while (args[i] != NULL)
	{
		if (is_valid_identifier(args[i]) == 0)
		{
			error_export = "minishell: export: `";
			write(STDERR_FILENO, error_export, ft_strlen(error_export));
			write(STDERR_FILENO, args[i], ft_strlen(args[i]));
			error_id = "': not a valid identifier\n";
			write(STDERR_FILENO, error_id, ft_strlen(error_id));
			status = 1;
		}
		else
		{
			eq_pos = ft_strchr(args[i], '=');
			if (eq_pos != NULL)
			{
				// Caso: export VAR=valor
				name = ft_substr(args[i], 0, eq_pos - args[i]);
				env_set(env, name, ft_strdup(eq_pos + 1), ENV);
			}
			else
			{
				// Caso: export VAR (sin valor)
				// Si existe, la convertimos a ENV. Si no, la ignoramos o creamos.	
				t_env_var *var = env_get_ptr(env, args[i]);
				if (var != NULL)
					var->type = ENV;
			}
		}
		i++;
	}
	return (status);
}
