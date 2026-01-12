#include "libft.h"

#include <linux/limits.h> /* For PATH_MAX */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static int	passwd_attempt(t_passwd *pwd, int fd, uid_t uid);
static int	fill_pwd_struct(t_passwd *pwd, char **ptokens, uid_t uid);
static int	home_attempt(t_passwd *pwd, uid_t uid);
static int	home_loop_body(t_passwd *pwd, struct dirent *entry,
	struct stat *st, uid_t uid);

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
 * function returns -1, it means only one thing:
 * malloc() failed or a system error occurred.
 * If returns 0 it means it could not retreive
 * the user's data. And 1 is returned on success */
int	ft_getpwuid(t_passwd *pwd, uid_t uid)
{
	int	fd;
	int	res;

	ft_memset(pwd, 0, sizeof(pwd));
	fd = open(PASSWD_PATH, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	res = passwd_attempt(pwd, fd, uid);
	if (res == 0)
		res = home_attempt(pwd, uid);
	if (close(fd) == -1)
	{
		perror("close");
		return (-1);
	}
	return (res);
}

/* On success returns 1, when failed to match UID it will return 0,
 * and on malloc() error -1.
 *     ptokens - passwd tokens ._. */
static int	passwd_attempt(t_passwd *pwd, int fd, uid_t uid)
{
	char	*line;
	char	**ptokens;
	
	line = get_next_line(fd);
	while (line)
	{
		ptokens	= ft_split(line, ':');
		if (ft_atoi(ptokens[2]) == (int)uid)
		{
			if (!fill_pwd_struct(pwd, ptokens, uid))
				return (-1);
			return (1);
		}
		split_free(&ptokens);
		free(line);
		line = get_next_line(fd);
	}
	if (!line)
	{
		write(STDERR_FILENO, GNL_ERR_MSG, ft_strlen(GNL_ERR_MSG));
		return (-1);
	}
	return (0);
}

static int	fill_pwd_struct(t_passwd *pwd, char **ptokens, uid_t uid)
{
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
		return (0);
	}
	return (1);
}

/* On malloc() error returns -1, if not found 0, and on success 1 */
static int	home_attempt(t_passwd *pwd, uid_t uid)
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		st;
	int				ret;

	dir = opendir(COMMON_HOME_DIR);
	if (!dir)
	{
		perror("opendir");
		return (-1);
	}
	errno = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		ret = home_loop_body(pwd, entry, &st, uid);
		if (ret)
			break ;
		entry = readdir(dir);
	}
	if (errno && !entry)
		perror("readdir");
	closedir(dir);
	return (ret);
}

static int	home_loop_body(t_passwd *pwd, struct dirent *entry,
	struct stat *st, uid_t uid)
{
	char	user_dir[PATH_MAX];

	if (stat(entry->d_name, st))
	{
		perror("stat");
		return (-1);
	}
	if (st->st_uid == uid)
	{
		pwd->pw_name = ft_strdup(entry->d_name);
		pwd->pw_uid = uid;
		ft_strlcpy(user_dir, COMMON_HOME_DIR, PATH_MAX);
		ft_strlcat(user_dir, "/", PATH_MAX);
		ft_strlcat(user_dir, entry->d_name, PATH_MAX);
		pwd->pw_dir = ft_strdup(user_dir);
		return (1);
	}
	return (0);
}
