#ifndef ENV_H
# define ENV_H

# include <stdbool.h>

# define ENV_VAR_NAME_CHK_SIZE	32
# define ENV_VAR_VAL_CHK_SIZE	64

typedef enum e_var_type
{
	LOCAL,		// MINISHELL_VERSION, HISTFILESIZE, HISTFILE, HISTSIZE, UID, EUID and etc.
	INHERIT,	// PATH, SHLVL, OLDPW, PWD, HOME, SHELL, USER and etc.
	PARAM		// $?, $$, $#, $*, $0, $1, $2, $3, $4, $5, $6, $7, $8, $9
}	t_var_type;

/* If `type` is PARAM, `value` points to
 * the corresponding argument in `argv`;
 *
 * If `type` is INHERIT, `value` points
 * to the corresponding value in `env`
 * or if it wasn't inherited it points
 * to the string we determine ourself;
 *
 * If `type` is LOCAL, `value` points to
 * the string we determine ourselfs.
 * */
typedef struct s_env_var
{
	char		*name;		// We'll just not have any limit here!
	char		*value;		// Here as well, no limit
	bool		f_readonly;
	t_var_type	type;

}	t_env_var;

int	init_var(t_var_type type, u_char perm);

#endif
