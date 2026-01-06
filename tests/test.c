#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int	main()
{
	if (unlink("file_that_does_not_exist") == -1)
	{
		char *err = strerror(errno);
		write(STDERR_FILENO, err, strlen(err));
	}
	return 0;
}
