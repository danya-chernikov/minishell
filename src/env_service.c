#include "env.h"
#include "error.h"

bool	env_exist(t_env *env, char *name)
{
	return (env_get_ptr(env, name));
}

/* Exports the variable named name. If no
 * variable with this name exists, an error
 * is returned. Parameter variables cannot
 * be exported. Only local variables created
 * during the session can be exported.
 * Technically, it is possible to export
 * 'special' local variables defined by our
 * shell, such as BASHPID, BASH_SUBSHELL,
 * BASH_VERSION, ... PPID, UID, and EUID.
 * However, even if these variables are
 * exported, they will be overwritten in
 * child shells immediately. Additionally,
 * descendant shells will read their
 * configuration files, and if a configuration
 * defines a 'special' local variable (except
 * PPID, UID, and EUID, which are always
 * determined internally), its value will be
 * changed */
int	env_export(t_env *env, char *name)
{
	t_env_var	*var;

	var = env_get_ptr(env, name);
	if (var)
	{
		if (var->type == LOCAL && var->f_inherit)
			var->type = ENV;
	}
	else
		return (COMMON_FAILURE);
	return (COMMON_SUCCESS);
}
