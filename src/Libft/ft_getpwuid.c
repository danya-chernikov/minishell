#include "libft.h"

#include <linux/limits.h> /* For PATH_MAX */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static int	passwd_attempt(t_passwd *pwd, int fd, uid_t uid);
static void	passwd_attempt_clearner(char ***ptokens, char *line, int fd);
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
	int		err;
	
	err = 0;
	line = get_next_line(fd, &err);
	while (line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		ptokens	= ft_split(line, ':');
		if (split_size(ptokens) > 2 && ft_atoi(ptokens[2]) == (int)uid)
		{
			if (!fill_pwd_struct(pwd, ptokens, uid))
			{
				passwd_attempt_clearner(&ptokens, line, fd);
				return (-1);
			}
			passwd_attempt_clearner(&ptokens, line, fd);
			return (1);
		}
		free(line);
		split_free(&ptokens);
		line = get_next_line(fd, &err);
	}
	if (!line && err)
	{
		write(STDERR_FILENO, GNL_ERR_MSG, ft_strlen(GNL_ERR_MSG));
		gnl_finish(fd); // I guess we need it also her
		split_free(&ptokens);
		return (-1);
	}
	gnl_finish(fd); // And here
	return (0);
}

static void	passwd_attempt_clearner(char ***ptokens, char *line, int fd)
{
	free(line);
	gnl_finish(fd);
	split_free(ptokens);
}

/* Returns -1 on a malloc() error, 0 if no match is
 * found, and 1 on success. The loop exits if
 * home_loop_body() returns -1 (indicating a malloc()
 * or system error) or if it returns 1, which means
 * the UID of the files represented by `st` matches
 * the provided `uid` */
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

/* Returns -1 on a malloc() error. If the UID of the file owner
 * represented by `st` matches `uid`, it returns 0. On success,
 * it returns 1 */
static int	home_loop_body(t_passwd *pwd, struct dirent *entry,
	struct stat *st, uid_t uid)
{
	char	user_dir[PATH_MAX];

	ft_strlcpy(user_dir, COMMON_HOME_DIR, PATH_MAX);
	ft_strlcat(user_dir, "/", PATH_MAX);
	ft_strlcat(user_dir, entry->d_name, PATH_MAX);
	if (!stat(user_dir, st))
	{
		if (st->st_uid == uid)
		{
			if (!fill_pwd_struct2(pwd, entry, user_dir, uid))
				return (-1);
			return (1);
		}
	}
	return (0);
}
