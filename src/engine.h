#ifndef ENGINE_H
# define ENGINE_H

# include "prompt_parser.h"
# include "exec.h"
# include "quote.h"
# include "debug.h"

int	shell_engine(char *prompt, int *ret_code);
int	comments_parser(t_parser_data *d);

#endif
