#ifndef ENGINE_H
# define ENGINE_H

# include "parser.h"
# include "exec.h"
# include "quote.h"
# include "debug.h"

int	shell_engine(char *prompt, int *ret_code);

#endif
