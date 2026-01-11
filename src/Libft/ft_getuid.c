#include "libft.h"

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int	ft_getuid(uid_t *uid)
{
	char		*term_name;
	ssize_t		ret;
	struct stat	sb;

	term_name = ttyname(STDIN_FILENO);
	if (!term_name)
	{
		perror("ttyname");
		return (-1);
	}
	ret = stat(term_name, &sb);
	if (ret)
	{
		perror("stat");
		return (-1);
	}
	*uid = sb.st_uid;
	return (0);
}
