#include "env.h"

/*
typedef struct s_env_var
{
	bool		f_readonly;
	t_var_type	type;
	char		*name;		// We'll just not have any limit here!
	char		*value;		// Here as well, no limit

}	t_env_var;

typedef enum e_paramvar
{
	PV_HOME,	// ~
	PV_RETCODE,	// $?
	PV_PID,		// $$
	PV_ARGNUM,	// $#
	PV_ALLARGS,	// $* Accessible only in scripts!
	PV_ARGV0,	// if equals '-minishell' our shell was run as login-shell
	PV_ARGV1,	// Only in scripts	
	PV_ARGV2,	// ...
	PV_ARGV3,	// ...
	PV_ARGV4,	// ...
	PV_ARGV5,	// ...
	PV_ARGV6,	// ...
	PV_ARGV7,	// ...
	PV_ARGV8,	// ...
	PV_ARGV9,	// Only in scripts
	
}	t_paramvar;

typedef enum e_var_type
{
	LOCAL,
	ENV,
	PARAM
}	t_var_type;

 * */
void	init_param_vars(t_env_var *vars)
{
	size_t	i;

	i = 0;
	while (i < PARAMVARS_NUM)
	{
		vars[i].type = PARAM;
		vars[i].f_readonly = true;
		vars[i].value = NULL;
		++i;
	}
	vars[PV_HOME].name		= ft_strdup("~");
	vars[PV_RETCODE].name	= ft_strdup("$?");
	vars[PV_PID].name		= ft_strdup("$$");
	vars[PV_ARGNUM].name	= ft_strdup("$#");
	vars[PV_ALLARGS].name	= ft_strdup("$*");
	vars[PV_ARGV0].name		= ft_strdup("$0");
	vars[PV_ARGV1].name		= ft_strdup("$1");
	vars[PV_ARGV2].name		= ft_strdup("$2");
	vars[PV_ARGV3].name		= ft_strdup("$3");
	vars[PV_ARGV4].name		= ft_strdup("$4");
	vars[PV_ARGV5].name		= ft_strdup("$5");
	vars[PV_ARGV6].name		= ft_strdup("$6");
	vars[PV_ARGV7].name		= ft_strdup("$7");
	vars[PV_ARGV8].name		= ft_strdup("$8");
	vars[PV_ARGV9].name		= ft_strdup("$9");
	i = 0;
	while (i < PARAMVARS_NUM)
	{
		if (!vars[i])
		{
			perror("malloc");
			return (0);
		}
		++i;
	}
	return (1);
}

void	free_param_vars(t_env_var *vars)
{
	size_t	i;

	i = 0;
	while (i < PARAMVARS_NUM)
	{
		if (vars[i].name)
			free(vars[i].name);
		if (vars[i].value)
			free(vars[i].value);
		++i;
	}
}
