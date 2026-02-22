#include "path_finder.h"

#if 0
COMMAND PATH RESOLUTION MODULE
This block contains helper functions to locate executable binaries within 
the systems PATH environment variable.

find_path_env - Searches for the "PATH=" variable in the environment.
@envp: Array of environment variables.
Return: A pointer to the start of the paths, or NULL if not found.
#endif

static char	*find_path_env(char **envp)
{
	//size_t path_var_name_len = ft_strlen(PATH_VAR_NAME);
	if (envp == NULL)
		return (NULL);
	while (*envp != NULL)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

static char	**ft_free_matrix(char **matrix)
{
	int	index;

	index = 0;
	if (matrix == NULL)
		return (NULL);
	while (matrix[index] != NULL)
	{
		free(matrix[index]);
		index++;
	}
	free(matrix);
	return (NULL);
}

char	*get_cmd_path(const char *cmd, char **envp)
{
	char	**paths; // Array de rutas (bin, usr/bin...)
	char	*path_env; // El string completo del PATH
	char	*full_path; // La ruta final probada (ej: /bin/ls)
	char	*part_path; // Auxiliar para añadir la barra '/'
	int	index;

	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = find_path_env(envp);
	if (path_env == NULL)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (paths == NULL)
		return (NULL);
	index = 0;
	while (paths[index] != NULL)
	{
		part_path = ft_strjoin(paths[index], "/");
		full_path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			ft_free_matrix(paths);
			return (full_path);
		}
		free(full_path);
		index++;	
	}
	ft_free_matrix(paths);
	return (NULL);
}
