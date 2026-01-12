#include "libft.h"

#include <stdio.h>
#include <stdint.h>

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

	ret = ft_getppid(&ppid);
	if (!ret)
		printf("My PPID is %jd\n", (intmax_t)ppid);
	
	ret = ft_getuid(&uid);
	if (!ret)
		printf("My UID is %d\n", uid);

	ret = ft_getgid(&gid);
	if (!ret)
		printf("My GID is %d\n", gid);

	ret = ft_getpwuid(&pwd, uid);
	if (ret)
	{
		printf("Username: %s", pwd.pw_name);
		printf("User password: %s", pwd.pw_passwd);
		printf("User ID: %d", pwd.pw_uid);
		printf("Group ID: %d", pwd.pw_gid);
		printf("User information: %s", pwd.pw_gecos);
		printf("Home directory: %s", pwd.pw_dir);
		printf("Shell program: %s", pwd.pw_shell);
	}
	free_pwd(&pwd);
	return 0;
}
