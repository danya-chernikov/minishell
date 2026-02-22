#include "env.h"

#include <stdio.h>

/* Prints the value of the varible named `name`.
 * If this variable does not exist, print a newline.
 * So it's quite similar to `echo $VAR` behaviour */
void	env_print_value(t_env *env, char *name)
{
	t_env_var	*var;

	var = env_get_ptr(env, name);
	if (var)
		printf("%s", var->value);
	printf("\n");
}

/* Basically, this is an implementation
 * of the `printenv` and `env` built-ins.
 * It prints only environment variables
 * that are inherited by our child
 * processes */
void	env_print_env(t_env *env)
{
	size_t	vi;
	
	vi = 0;	
	while (vi < env->vars_num)
	{
		if (env->vars[vi].name && env->vars[vi].type == ENV)
			printf("%s=%s\n", env->vars[vi].name, env->vars[vi].value);
		++vi;
	}
}

/* Implementation of the `set` built-in
 * without any arguments. Prints both
 * all local variables and all
 * environment variables */
void	env_print_all(t_env *env)
{
	size_t	vi;
	
	vi = 0;	
	while (vi < env->vars_num)
	{
		if (env->vars[vi].name && env->vars[vi].type != PARAM)
			printf("%s=%s\n", env->vars[vi].name, env->vars[vi].value);
		++vi;
	}
}

/* I honestly have no idea
 * why bash does not provide
 * a built-in command to print
 * only the local variables
 * we create during a session.
 * Well.. this function does
 * exactly that wuw */
void	env_print_locals(t_env *env)
{
	size_t	vi;
	
	vi = 0;	
	while (vi < env->vars_num)
	{
		if (env->vars[vi].name && env->vars[vi].type == LOCAL)
			printf("%s=%s\n", env->vars[vi].name, env->vars[vi].value);
		++vi;
	}
}
