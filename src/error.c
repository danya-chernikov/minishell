#include "error.h"
#include "shell.h"

/* We use it when parsing the prompt */
void	handle_prompt_parser_error(bool *f_noerr)
{
	char	err_buf[ERR_BUF_SIZE];

	*f_noerr = false;
	ft_strlcpy(err_buf, PARSER_ERR_MSG, ERR_BUF_SIZE);
	ft_strlcat(err_buf, "\n", ERR_BUF_SIZE);
	write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
}

/* Sets error flag and prints extended parsing error message */
void	handle_prompt_parser_ext_error(bool *f_noerr, char *msg)
{
	char	err_buf[ERR_BUF_SIZE];

	*f_noerr = false;
	ft_strlcpy(err_buf, PARSER_ERR_MSG, ERR_BUF_SIZE);
	ft_strlcat(err_buf, ": ", ERR_BUF_SIZE);
	ft_strlcat(err_buf, msg, ERR_BUF_SIZE);
	ft_strlcat(err_buf, "\n", ERR_BUF_SIZE);
	write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
}

/* Just prints extended parsing error message */
void	print_prompt_parser_ext_error(char *msg)
{
	char	err_buf[ERR_BUF_SIZE];

	ft_strlcpy(err_buf, PARSER_ERR_MSG, ERR_BUF_SIZE);
	ft_strlcat(err_buf, ": ", ERR_BUF_SIZE);
	ft_strlcat(err_buf, msg, ERR_BUF_SIZE);
	ft_strlcat(err_buf, "\n", ERR_BUF_SIZE);
	write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
}

void	print_sys_error(char *msg)
{
	char	err_buf[ERR_BUF_SIZE];
	char	*err;

	err = strerror(errno);
	ft_strlcpy(err_buf, msg, ERR_BUF_SIZE);
	ft_strlcat(err_buf, ": ", ERR_BUF_SIZE);
	ft_strlcat(err_buf, err, ERR_BUF_SIZE);
	ft_strlcat(err_buf, "\n", ERR_BUF_SIZE);
	write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
}

void	print_shell_error(char *culprit, char *msg)
{
	char	err_buf[ERR_BUF_SIZE];

	ft_strlcpy(err_buf, MSH_NAME_SHORT, ERR_BUF_SIZE);
	ft_strlcat(err_buf, ": ", ERR_BUF_SIZE);
	if (culprit)
	{
		ft_strlcat(err_buf, culprit, ERR_BUF_SIZE);
		ft_strlcat(err_buf, ": ", ERR_BUF_SIZE);
	}
	ft_strlcat(err_buf, msg, ERR_BUF_SIZE);
	ft_strlcat(err_buf, "\n", ERR_BUF_SIZE);
	write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
}
