#include "builtin.h"
#include "env.h"

/*
static char	*ft_strchr(char *str, int c)
{
	int	index;

	index = 0;
	while ((str[index] != c) && (str[index] != '\0'))
		index++;
	if (str[index] == c)
		return (str + index);
	else
		return (NULL);
}

static size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str != '\0')
	{
		len++;
		str++;
	}
	return (len);
}

int     ft_env(char **envp)
{
        int     index;

        index = 0;
        if (envp == NULL)
                return (0);
        while (envp[index] != NULL)
        {
                if (ft_strchr(envp[index], '=') != NULL)
                {
                        write(STDOUT_FILENO, envp[index], ft_strlen(envp[index]));
                        write(STDOUT_FILENO, "\n", 1);
                }
                index++;
        }
        return (0);
}
*/

int	ft_env(t_env *env)
{
	if (env == NULL)
		return (1);
	env_print_env(env);
	return (0);
}
