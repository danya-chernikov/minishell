#include "libft.h"

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int	ft_getgid(gid_t *gid)
{
	char		*term_name;
	ssize_t		ret;
	struct stat	st;

	term_name = ttyname(STDIN_FILENO);
	if (!term_name)
	{
		perror("ttyname");
		return (-1);
	}
	ret = stat(term_name, &st);
	if (ret)
	{
		perror("stat");
		return (-1);
	}
	*gid = st.st_gid;
	return (0);
}
