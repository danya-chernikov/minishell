#include "libft.h"
#include <stdlib.h>

void	split_free(char ***res)
{
	size_t	i;

	i = 0;
	while ((*res)[i])
	{
		free((*res)[i]);
		++i;
	}
	free(*res);
}

void	free_pwd(t_passwd *pwd)
{
	if (pwd->pw_name)
		free(pwd->pw_name);
	if (pwd->pw_passwd)
		free (pwd->pw_passwd);
	if (pwd->pw_gecos)
		free (pwd->pw_gecos);
	if (pwd->pw_dir)
		free (pwd->pw_dir);
	if (pwd->pw_shell)
		free (pwd->pw_shell);
}
