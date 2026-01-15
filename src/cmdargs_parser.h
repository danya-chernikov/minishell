#ifndef CMDARGS_PARSER_H
# define CMDARGS_PARSER_H

#include "shell.h"

int		cmdargs_parser(t_shell *msh);
int		set_argv0_login(t_shell *msh);

bool	is_help_opt(char *opt);
bool	is_version_opt(char *opt);
bool	is_verbose_opt(char *opt);
bool	is_login_opt(char *opt);
bool	is_norc_opt(char *opt);
bool	is_s_opt(char *opt);

void	print_help(void);
void	print_version(void);

#endif
