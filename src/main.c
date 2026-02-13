#include "shell.h"
#include "engine.h"

/* The main() may return one of the other
 * possible errors described in the `e_exit_code`
 * structure, or 128 + N, where N is the number
 * of the signal that interrupted our minishell */
int	main(int argc, char **argv, char **env)
{
	t_shell	msh;
	int		fret;
	int		shret;

	shret = EXIT_SUCCESS;

	fret = msh_init(&msh, argc, argv, env);
	if (fret == COMMON_SYS_ERR) // -1
		shret = SYS_ERR; // 1
	else
		shret = fret;

	fret = msh_launch(&msh);
	if (fret == COMMON_SYS_ERR) // -1
		shret = SYS_ERR; // 1
	else
		shret = fret;

	msh_free(&msh);
	return (shret);
}
