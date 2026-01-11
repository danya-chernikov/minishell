#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include "libft.h"

#define PASSWD_PATH	"/etc/passwd"

void split_free(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
}

int	main()
{
	char		*term_name;
	struct stat	sb;
	ssize_t		ret;
	uid_t		my_uid;
	gid_t		my_gid;
	int			fd;
	char		*line;
	char		**ptokens; // dasswd tokens ._.

	term_name = ttyname(STDIN_FILENO);
	if (!term_name)
	{
		perror("ttyname");
		return 1;
	}
	ret = stat(term_name, &sb);
	if (ret)
	{
		perror("stat");
		return 1;
	}
	my_uid = sb.st_uid;
	my_gid = sb.st_gid;
	printf("My UID is %d\n", my_uid);
	printf("My GID is %d\n", my_gid);

	fd = open(PASSWD_PATH, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return 1;
	}
	do
	{
		line = get_next_line(fd);
		if (line)
		{
			ptokens	= ft_split(line, ':');
			if (ft_atoi(ptokens[2]) == my_uid)
			{
				printf("My username is %s\n", ptokens[0]);
				printf("My home directory is %s\n", ptokens[5]);
			}
			split_free(ptokens);
		}
		free(line);
	} while (line);

	if (close(fd) == -1)
	{
		perror("close");
		return 1;
	}
	return 0;
}
