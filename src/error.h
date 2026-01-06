#ifndef ERROR_H
# define ERROR_H

# include <unistd.h>
# include <errno.h>
# include <stdbool.h>
# include <string.h>

# include "libft.h"

# define PARSER_ERR_MSG	"Parsing error\n"
# define ERR_BUF_SIZE	256

void	parser_error(bool *f_noerr);
void	print_sys_error(char *msg);

#endif
