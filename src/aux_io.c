#include "aux_io.h"
#include "error.h"

#include <unistd.h>

int	write_all(int fd, const char *buf, size_t n)
{
	ssize_t	wlen;
	size_t	offset;

	offset = 0;
	while (offset < n)
	{
		wlen = write(fd, buf + offset, n - offset);
		if (wlen < 0)
			return (COMMON_SYS_ERR);
		offset += (size_t)wlen;
	}
	return (COMMON_SUCCESS);
}
