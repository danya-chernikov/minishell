#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int main()
{
	pid_t		pid;
	pid_t		ppid;
	uid_t		uid;
	gid_t		gid;
	t_passwd	pwd;
	int			ret;

	ret = ft_getpid(&pid);
	if (!ret)
		printf("My PID is %jd\n", (intmax_t)pid);
	printf("Kernel Linux PID: %jd\n\n", (intmax_t)getpid());

	ret = ft_getppid(&ppid);
	if (!ret)
		printf("My PPID is %jd\n", (intmax_t)ppid);
	printf("Kernel Linux PPID: %jd\n\n", (intmax_t)getppid());
	
	ret = ft_getuid(&uid);
	if (!ret)
		printf("My UID is %d\n", uid);
	printf("Kernel Linux UID: %d\n\n", getuid());

	ret = ft_getgid(&gid);
	if (!ret)
		printf("My GID is %d\n", gid);
	printf("Kernel Linux GID: %d\n\n", getgid());

	ret = ft_getpwuid(&pwd, uid);
	if (ret == 0)
	{
		printf("ft_getpwuid(): Could not determine who're you!\n");
	}
	if (ret == 1)
	{
		if (pwd.pw_name)
			printf("Username: %s\n\n", pwd.pw_name);

		if (pwd.pw_passwd)
			printf("User password: %s\n\n", pwd.pw_passwd);

		printf("User ID: %d\n\n", pwd.pw_uid);
		printf("Group ID: %d\n\n", pwd.pw_gid);

		if (pwd.pw_gecos)
			printf("User information: %s\n\n", pwd.pw_gecos);

		if (pwd.pw_dir)
			printf("Home directory: %s\n\n", pwd.pw_dir);

		if (pwd.pw_shell)
			printf("Shell program: %s\n\n", pwd.pw_shell);
	}
	else if (ret == -1)
	{
		exit(EXIT_FAILURE);
	}
	free_pwd(&pwd);
	return 0;
}
