#include "shell.h"

void	print_help(void)
{
	printf("minishell, version %s-(%s-pc-%s)\n",
		MSH_VERSION, MSH_ARCH, MSH_OSTYPE,
		"Usage:\tminishell [GNU long option] [option] ...\n"
		"\tminishell [GNU long option] [option] script-file ...\n"
		"GNU long options:\n"
		"\t--help\n"
		"\t--login\n"
		"\t--norc\n"
		"\t--verbose\n"
		"\t--version\n"
		"Shell options:\n"
		"\t-clv\n");
}
