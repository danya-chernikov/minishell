#include "builtin.h"
#include "env.h"

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

int	ft_strncmp(const char *str1, const char *str2, size_t nbr)
{
	size_t	index;

	index = 0;
	while (index < nbr && (str1[index] != '\0' || str2[index] != '\0'))
	{
		if (str1[index] != str2[index])
		{
			return ((unsigned char)str1[index] - (unsigned char)str2[index]);
		}
		index++;
	}
	return (0);
}

static void	error_msg_cd(const char *path)
{
	const char	*msg_error;

	write(STDERR_FILENO, "minishell: cd: ", 15);
	write(STDERR_FILENO, path, ft_strlen(path));
	write(STDERR_FILENO, ": ", 2);
	msg_error = strerror(errno);
	write(STDERR_FILENO, msg_error, ft_strlen(msg_error));
	write(STDERR_FILENO, "\n", 1);
}

static char	*ft_strcpy(char *s1, char *s2)
{
	char	*inicio_s1;

	inicio_s1 = s1;
	while (*s2 != '\0')
	{
		*s1 = *s2;
		s1++;
		s2++;
	}
	*s1 = '\0';
	return (inicio_s1);
}

static char	*ft_strdup(char *str)
{
	char	*copia;
	int	longitud;

	longitud = ft_strlen(str);
	copia = malloc(sizeof(char) * (longitud + 1));
	if (copia == NULL)
		return (NULL);
	copia = ft_strcpy(copia, str);
	return (copia);
}

/*
static char	*get_env_val(char **envp, char *var)
{
	int	index;
	int	len;

	index = 0;
	if (envp == NULL || var == NULL)
		return (NULL);
	len = ft_strlen(var);
	while (envp[index] != NULL)
	{
		if (ft_strncmp(envp[index], var, len) == 0 && envp[index][len] == '=')
			return (envp[index] + len + 1);
		index++;
	}
	return (NULL);
}
*/
int	ft_cd(char **args, t_env *env)
{
	char	*path;
	char	cwd[PATH_MAX];
	char	*error_getcwd;
	char	old_cwd[PATH_MAX];
	char	*error_home;
	char	*error_oldpwd;
	char	*error_pwd;

	if (getcwd(old_cwd, PATH_MAX) == NULL)
	{
		error_getcwd = "minishell: cd: getcwd failed";
		write(STDERR_FILENO, error_getcwd, ft_strlen(error_getcwd));
		return (1);
	}
	if (args[1] == NULL)
	{
		path = env_get_val(env, "HOME");
		if (path == NULL)
		{
			error_home = "minishell: cd: HOME not set\n";
			write(STDERR_FILENO, error_home, ft_strlen(error_home));
			return (1);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = env_get_val(env, "OLDPWD");
		if (path == NULL)
		{
			error_oldpwd = "minishell: cd: OLDPWD not set\n";
			write(STDERR_FILENO, error_pwd, ft_strlen(error_pwd));
			return (1);
		}
		write(STDOUT_FILENO, path, ft_strlen(path));
		write(STDOUT_FILENO, "\n", 1);
	}
	else 
		path = args[1];
	if (chdir(path) == -1)
	{
		error_msg_cd(path);
		return (1);
	}
	if (getcwd(cwd, PATH_MAX) != NULL)
	{
		env_set(env, "OLDPWD", ft_strdup(old_cwd), ENV);
		env_set(env, "PWD", ft_strdup(cwd), ENV);
	}
	else
	{
		error_getcwd = "minishell: cd: post-chdir getcwd failed";
		write(STDERR_FILENO, error_getcwd, ft_strlen(error_getcwd));
		return (1);
	}
	return (0);
}
