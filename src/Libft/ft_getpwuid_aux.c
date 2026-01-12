#include "libft.h"

#include <stdio.h>

int	fill_pwd_struct(t_passwd *pwd, char **ptokens, uid_t uid)
{
	if (split_size(ptokens) < 7)
		return (0);
	pwd->pw_name = ft_strdup(ptokens[0]);
	pwd->pw_passwd = ft_strdup(ptokens[1]);
	pwd->pw_uid = uid;
	pwd->pw_gid = ft_atoi(ptokens[3]);
	pwd->pw_gecos = ft_strdup(ptokens[4]);
	pwd->pw_dir = ft_strdup(ptokens[5]);
	pwd->pw_shell = ft_strdup(ptokens[6]);
	if (!pwd->pw_name || !pwd->pw_passwd ||
		!pwd->pw_gecos || !pwd->pw_dir ||
		!pwd->pw_shell)
	{
		perror("malloc");
		return (0);
	}
	return (1);
}

int	fill_pwd_struct2(t_passwd *pwd, struct dirent *entry, char *user_dir, uid_t uid)
{
	int	ret;

	pwd->pw_name = ft_strdup(entry->d_name);
	pwd->pw_passwd = ft_strdup("");
	pwd->pw_uid = uid;
	ret = ft_getgid(&pwd->pw_gid);
	if (ret)
		return (0);
	pwd->pw_gecos = ft_strdup("");
	pwd->pw_dir = ft_strdup(user_dir);
	pwd->pw_shell = ft_strdup("");
	if (!pwd->pw_name || !pwd->pw_passwd ||
		!pwd->pw_gecos || !pwd->pw_dir ||
		!pwd->pw_shell)
	{
		perror("malloc");
		return (0);
	}
	return (1);
}
