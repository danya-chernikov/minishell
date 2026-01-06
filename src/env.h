#ifndef ENV_H
# define ENV_H

# include <stdbool.h>

# define ENV_VAR_NAME_CHK_SIZE	32
# define ENV_VAR_VAL_CHK_SIZE	64

typedef enum e_var_type
{
	LOCAL_VAR,
	GLOBAL_VAR
}	t_var_type;

typedef struct s_env_var
{
	char		*name;	// We'll just not have any limit here!
	char		*value;	// Here as well, no limit
	u_char		perm;	// Permissions
	t_var_type	type;
}	t_env_var;

int	init_var(t_var_type type, u_char perm);


#endif
