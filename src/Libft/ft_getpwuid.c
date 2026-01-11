#include "libft.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

static int	passwd_attempt(int fd, struct t_passwd *pwd, uid_t uid);
static int	fill_pwd_struct(struct t_passwd *pwd, char **ptokens, uid_t uid);
static int	home_attempt(int fd, struct t_passwd *pwd, uid_t uid);

/* Attempts to fill the pwd structure with
 * the parsed fields of the record in
 * `/etc/passwd` that matches the `uid`
 * passed as the second argument. If
 * `/etc/passwd` does not contain a record
 * with this UID, it attempts to open each
 * directory in /home and determine the owner
 * UID of each directory using stat(). It then
 * compares each UID with the given `uid` to
 * find the corresponding username. If this
 * also fails, it sets UNKNOWN_STR for the
 * fields of pwd that could not be resolved.
 * If this function returns -1, it means only
 * one thing: malloc() failed or a system error
 * occurred. If returns 1 it means it could not
 * retreive the user's data */
int	ft_getpwuid(struct t_passwd *pwd, uid_t uid)
{
	int		fd;

	ft_memset(pwd, 0, sizeof(pwd));
	fd = open(PASSWD_PATH, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return 1;
	}
	if (!passwd_attempt(fd, pwd, uid))
	{
		if (!home_attempt(fd, wd, uid))
		{

		}
	}
	if (close(fd) == -1)
	{
		perror("close");
		return 1;
	}
	return 0;
}

/* On success returns 1, on failure 0 */
static int	passwd_attempt(int fd, struct t_passwd *pwd, uid_t uid)
{
	char	*line;
	char	**ptokens; // passwd tokens ._.	
	
	line = get_next_line(fd);
	while (line)
	{
		ptokens	= ft_split(line, ':');
		if (ft_atoi(ptokens[2]) == uid)
		{
			if (!fill_pwd_struct(pwd, ptokens, uid))
				return (0);
			return (1);
		}
		split_free(ptokens);
		free(line);
		line = get_next_line(fd);
	}
	if (!line)
	{
		write(STDERR_FILENO, GNL_ERR_MSG, ft_strlen(GNL_ERR_MSG));
		return (0);
	}
	return (1);
}

static int	fill_pwd_struct(struct t_passwd *pwd, char **ptokens, uid_t uid)
{
	pwd.pw_name = ft_strdup(ptokens[0]);
	pwd.pw_passwd = ft_strdup(ptokens[1]);
	pwd->pw_uid = uid;
	pwd.pw_gid = ft_atoi(ptokens[3]);
	pwd.pw_gecos = ft_atoi(ptokens[4]);
	pwd.pw_dir = ft_atoi(ptokens[5]);
	pwd.pw_shell = ft_atoi(ptokens[6]);
	if (!pwd.pw_name || !pwd.pw_passwd ||
		!pwd->pw_gecos || !pwd.pw_dir ||
		!pwd.pw_shell)
	{
		return (0);
	}
	return (1);
}

static int	home_attempt(int fd, struct t_passwd *pwd, uid_t uid)
{

}
