#include "error.h"

void	parser_error(bool *f_noerr)
{
	*f_noerr = false;
	write(STDERR_FILENO, PARSER_ERR_MSG, ft_strlen(PARSER_ERR_MSG));
}

void	print_sys_error(char *msg)
{
	char	err_buf[ERR_BUF_SIZE];
	char	*err;

	err = strerror(errno);
	ft_strlcpy(err_buf, msg, ERR_BUF_SIZE);
	ft_strlcat(err_buf, ": ", 2);
	ft_strlcat(err_buf, err, ft_strlen(err));
	ft_strlcat(err_buf, "\n", 1);
	write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
}
