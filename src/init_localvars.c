#include "shell.h"

static void	prelim_vars_init(t_env *env);
static void	set_vars(t_env *env, char **argv, int *ret_codes);
static int	check_mem_errors(t_env *env);

int	msh_set_local_vars(t_env *env, char **argv)
{
	int	ret_codes[SLOCAL_VARS_NUM];
	int	ret_i;

	prelim_vars_init(env);
	set_vars(env, argv, ret_codes);
	ret_i = 0;
	while (ret_i < SLOCAL_VARS_NUM)
	{
		if (ret_codes[ret_i] == COMMON_SYS_ERR)
			return (COMMON_SYS_ERR);
		++ret_i;
	}	
	env->vars_num += SLOCAL_VARS_NUM;
	// Check for memory errors
	if (check_mem_errors(env) == COMMON_SYS_ERR)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}

static void	prelim_vars_init(t_env *env)
{
	t_slocalvar	vi;

	vi = SL_PPID;
	while (vi < SL_MSHSUBSH)
	{
		env->vars[vi].type = LOCAL;
		env->vars[vi].f_readonly = true;
		env->vars[vi].f_inherit = false;
		++vi;
	}
	while (vi < PARAM_VARS_NUM + SLOCAL_VARS_NUM)
	{
		env->vars[vi].type = LOCAL;
		env->vars[vi].f_readonly = false;
		env->vars[vi].f_inherit = false;
		++vi;
	}
}

static void	set_vars(t_env *env, char **argv, int *ret_codes)
{
	ret_codes[0] = set_local_ppid(env);			// PPID
	ret_codes[1] = set_local_uid(env);			// UID
	ret_codes[2] = set_local_euid(env);			// EUID
	ret_codes[3] = set_local_mshpid(env);		// MSHPID
	ret_codes[4] = set_local_mshsubsh(env);		// MSH_SUBSHELL
	ret_codes[5] = set_local_mshversion(env);	// MSH_VERSION
	ret_codes[6] = set_local_histfilesize(env);	// HISTFILESIZE
	ret_codes[7] = set_local_histfile(env);		// HISTFILE
	ret_codes[8] = set_local_histsize(env);		// HISTSIZE
	ret_codes[9] = set_local_msh(env, argv);	// MSH
	ret_codes[10] = set_local_hostname(env);	// HOSTNAME
	ret_codes[11] = set_local_hosttype(env);	// HOSTTYPE
	ret_codes[12] = set_local_ostype(env);		// OSTYPE
	ret_codes[13] = set_local_hostmach(env);	// HOSTMACH
	ret_codes[14] = set_local_ps1(env);			// PS1
	ret_codes[15] = set_local_ps2(env);			// PS2
	ret_codes[16] = set_local_ps4(env);			// PS4
}

static int	check_mem_errors(t_env *env)
{
	vi = SL_PPID;
	while (vi < SL_PPID + SLOCAL_VARS_NUM)
	{
		if (!env->vars[vi].name || !env->vars[vi].value)
		{
			perror("malloc");
			return (COMMON_SYS_ERR);
		}
		++vi;
	}
	return (COMMON_SUCCESS);
}
