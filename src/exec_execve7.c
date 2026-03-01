#include "exec.h"
#include "shell.h"
#include "env.h"

#include <stdlib.h>

size_t	count_shell_envp(t_shell *msh, t_env *op_env)
{
	size_t	i;
	size_t	cnt;

	i = 0;
	cnt = 0;
	while (i < msh->env.vars_num)
	{
		if (msh->env.vars[i].name &&
			msh->env.vars[i].type == ENV &&
			(!op_env || !env_exist(op_env, msh->env.vars[i].name)))
			++cnt;
		++i;
	}
	return (cnt);
}

char	*key_value_to_str(const char *key, const char *value)
{
	size_t	key_len;
	size_t	val_len;
	size_t	total_len;
	char	*str;
	
	key_len = ft_strlen(key);
	val_len = 0;
	if (value)
		val_len = ft_strlen(value);
	total_len = key_len + val_len + 2;
	str = (char *)malloc(total_len * sizeof (char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, key, total_len);
	ft_strlcat(str, "=", total_len);
	if (value)
		ft_strlcat(str, value, total_len);
	return (str);
}
