#ifndef CMDARGS_PARSER_H
# define CMDARGS_PARSER_H

# define LOGIN_LONG_OPT		"--login"
# define VERBOSE_LONG_OPT	"--verbose"
# define NORC_LONG_OPT		"--norc"
# define HELP_LONG_OPT		"--help"
# define LOGIN_SHORT_OPT	"-l"
# define VERBOSE_SHORT_OPT	"-v"
# define C_SHORT_OPT		"-c"

# include "shell.h"

# include <stdbool.h>

/* cmdargs_parser.c */
int		cmdargs_parser(t_shell *msh);
int		handle_c_opt(t_shell *msh, int opt_i);
int		handle_script(t_shell *msh, int opt_i);

/* cmdargs_parser2.c */
int		set_script_args(t_shell *msh, int opt_i, size_t *allargv_len, int *arg_i);
int		set_allargs_var(t_shell *msh, size_t allargv_len, int arg_i);
int		set_argv0_login(t_shell *msh);
int		erase_quotes(char *str);
void	get_last_path_comp(char *path);

/* cmdargs_parser3.c */
bool	is_help_opt(char *opt);
bool	is_version_opt(char *opt);
bool	is_verbose_opt(char *opt);
bool	is_login_opt(char *opt);
bool	is_norc_opt(char *opt);

/* cmdargs_parser4.c */
bool	is_c_opt(char *opt);
void	print_help(void);
void	print_version(void);

#endif
