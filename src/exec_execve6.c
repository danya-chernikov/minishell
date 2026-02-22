#include "exec.h"
#include "builtin.h"
#include "aux_common.h"

bool	is_parent_builtin(const char *s)
{
	if (!s)
		return (false);
	if (strings_equal((char *)s, CD_CMD))
		return (true);
	if (strings_equal((char *)s, EXPORT_CMD))
		return (true);
	if (strings_equal((char *)s, UNSET_CMD))
		return (true);
	if (strings_equal((char *)s, EXIT_CMD))
		return (true);
	return (false);
}

bool	is_any_builtin(const char *s)
{
	if (!s)
		return (false);
	if (strings_equal((char *)s, ECHO_CMD))
		return (true);
	if (strings_equal((char *)s, CD_CMD))
		return (true);
	if (strings_equal((char *)s, PWD_CMD))
		return (true);
	if (strings_equal((char *)s, EXPORT_CMD))
		return (true);
	if (strings_equal((char *)s, UNSET_CMD))
		return (true);
	if (strings_equal((char *)s, ENV_CMD))
		return (true);
	if (strings_equal((char *)s, PRINTENV_CMD))
		return (true);
	if (strings_equal((char *)s, EXIT_CMD))
		return (true);
	if (strings_equal((char *)s, TRUE_CMD))
		return (true);
	if (strings_equal((char *)s, FALSE_CMD))
		return (true);
	if (strings_equal((char *)s, NOP_CMD))
		return (true);
	return (false);
}

void	child_set_default_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
