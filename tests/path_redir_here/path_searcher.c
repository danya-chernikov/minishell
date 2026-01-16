#define DEF_PATH   "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
#define PROG_BUF	1024

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int main()
{
	char prog[PROG_BUF];
	scanf("%s", prog);
	if (prog[0] == '.')
	{
		// Search in relative routes
		// ...
		// getcwd()
		// chcwd()
		// opendir()
	}
	else
	{
		// Search in PATH
		// ...
		// opendir()
	}
	return 0;
}
