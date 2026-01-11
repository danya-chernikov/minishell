#include "libft.h"

#include <stdio.h>
#include <stdint.h>

int main()
{
	pid_t	pid;
	pid_t	ppid;
	uid_t	uid;
	gid_t	gid;
	int		ret;	

	ret = ft_getpid(&pid);
	if (!ret)
		printf("My PID is %jd\n", (intmax_t)pid);

	ret = ft_getppid(&ppid);
	if (!ret)
		printf("My PPID is %jd\n", (intmax_t)ppid);
	
	ret = ft_getuid(&uid);
	if (!ret)
		printf("My UID is %d\n", uid);

	ret = ft_getgid(&gid);
	if (!ret)
		printf("My GID is %d\n", gid);

	return 0;
}
