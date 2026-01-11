#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "libft.h"

#define PROC_INFO_SOURCE	"/proc/self/status"
#define PROC_PID_TOKEN		"Pid"
#define PROC_PPID_TOKEN		"PPid"

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
	int		fd;
	int		proc_pid;
	int		proc_ppid;
	char	*line;
	char	**ptokens; // Process tokens ._.
	size_t	tlen;

	printf("My PID from getpid() is %jd\n", (intmax_t)getpid());
	printf("My PPID from getppid() is %jd\n", (intmax_t)getppid());
	fd = open(PROC_INFO_SOURCE, O_RDONLY);
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
			tlen = ft_strlen(PROC_PID_TOKEN);
			if (!ft_strncmp(ptokens[0], PROC_PID_TOKEN, tlen) &&
				ft_strlen(ptokens[0]) == tlen)
			{
				proc_pid = ft_atoi(ptokens[1]);
				printf("My PID form %s is %d\n", PROC_INFO_SOURCE, proc_pid);
			}
			tlen = ft_strlen(PROC_PPID_TOKEN);
			if (!ft_strncmp(ptokens[0], PROC_PPID_TOKEN, tlen) &&
				ft_strlen(ptokens[0]) == tlen)
			{
				proc_ppid = ft_atoi(ptokens[1]);
				printf("My PPID form %s is %d\n", PROC_INFO_SOURCE, proc_ppid);
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
