#include "builtin.h"

#if 0
Devuelve 1 si el comando es un built-in, 0 si no.
Útil para que el ejecutor sepa si debe hacer fork o no.
#endif

static int      ft_strcmp(char *str1, char *str2)
{
        while ((*str1 != '\0' || *str2 != '\0') && (*str1 == *str2))
        {
                str1++;
                str2++;
        }
        return (*str1 - *str2);
}

int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_strcmp(cmd, ECHO_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, CD_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, PWD_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, EXPORT_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, UNSET_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, ENV_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, EXIT_CMD) == 0)
		return (1);
	return (0);
}

#if 0
Ejecuta el built-in
#endif
int	exec_builtin(char **args, t_env *env)
{
	if (args == NULL || args[0] == NULL)
		return (0);
	if (ft_strcmp(args[0], ECHO_CMD) == 0)
		return (ft_echo(args));
	if (ft_strcmp(args[0], CD_CMD) == 0)
		return (ft_cd(args, env));
	if (ft_strcmp(args[0], PWD_CMD) == 0)
		return (ft_pwd());
	if (ft_strcmp(args[0], EXPORT_CMD) == 0)
		return (ft_export(args, env));
	if (ft_strcmp(args[0], UNSET_CMD) == 0)
		return (ft_unset(args, env));
	if (ft_strcmp(args[0], ENV_CMD) == 0)
		return (ft_env(env));
	if (ft_strcmp(args[0], EXIT_CMD) == 0)
		return (ft_exit(args));
	return (127);
}
