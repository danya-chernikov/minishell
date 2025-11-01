#include <stdio.h>

/* In bash we should be able to run:
 *     ./env_example # its output must be identical to 'env')
 *     VAR1=1 ./env_example # we just add one new environment variable
 *     VAR2=1 VAR3=44:11 ./env_example
 *     VAR2=1 VAR3=44:12 ./env_example (this will change the previous variable's value)
 *     VAR2=1 VAR3=44:12 VAR4=33:"lololo" ./env_example
 *     VAR2=1 VAR3=44:12 VAR4=33:"lololo kkeke" ./env_example
 *     env VAR2=100 VAR3=44:12 VAR4=33:"lololo kkeke" ./env_example (adding env has the same effect);
 *     VAR2=1 VAR3=44:12 VAR4=33:"lololo ::::kkeke" ./env_example
 *     VAR2=1 VAR3=44:12 VAR4=33:"lolol""" ./env_example
 *     VAR2=1 VAR3=44:12 VAR4=33:"lolol""efef" ./env_example
 *
 * */
int	main(int argc, char **argv, char **env)
{
	for (int i = 0; env[i]; ++i)
		printf("%d. %s\n", i + 1, env[i]);
	return 0;
}
