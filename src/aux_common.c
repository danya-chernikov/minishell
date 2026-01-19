#include "aux_common.h"

bool	strings_equal(char *str1, char *str2)
{
	if (ft_strlen(str1) == ft_strlen(str2) &&
		!ft_strncmp(str1, str2, ft_strlen(str1)))
		return (true);
	return (false);
}

/* Does the same as ft_split(), but only for two tokens, i.e. it splits `str`
 * into two substrings, `part1` and `part2`, using the delimiter `delim` */
int div2_str_by_delim(char *str, char delim, char **part1, char **part2)
{
    size_t  i;

    if (strlen(str) - 1 > MAX_ENV_NAME_LEN + MAX_ENV_VAL_LEN)
        return (COMMON_FAILURE);
	if (div2_str_by_delim_alloc(part1, part2) == COMMON_SYS_ERR)
        return (COMMON_SYS_ERR);
    i = 0;
    while (str[i] && str[i] != delim)
	{
        (*part1)[i] = str[i];
		++i;
	}
    (*part1)[i] = '\0';
	if (!(*part2))
		return (COMMON_SUCCESS);
	++i;
    while (str[i])
	{
        (*part2)[i - strlen(*part1) - 1] = str[i];
		++i;
	}
    (*part2)[i - strlen(*part1) - 1] = '\0';
    return (COMMON_SUCCESS);
}

int	div2_str_by_delim_alloc(char **part1, char **part2)
{
    *part1 = (char *)malloc(MAX_ENV_NAME_LEN * sizeof(char));
    if (!(*part1))
        return (COMMON_SYS_ERR);
    *part2 = (char *)malloc(MAX_ENV_VAL_LEN * sizeof(char));
    if (!(*part2))
    {
        free(*part1);
        return (COMMON_SYS_ERR);
    }
	return (COMMON_SUCCESS);
}

// DOUBT WE NEED IT ANYMORE!
/* Checks whether the variable at index `i`
 * in `env->inh_env` has already been created
 * in the special local variables section */
bool	in_locals(t_env *env, char *var_name)
{
	int			res;
	t_paramvar	vi;

	vi = PV_HOME;
	while (vi < PV_HOME + PARAM_VARS_NUM)
	{
		if (strings_equal(var_name, env->vars[vi]))
			return (true); // Exists
		++vi;
	}
	return (false);
}
