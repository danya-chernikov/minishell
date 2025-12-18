#include <stdio.h>

int	main(void)
{
	printf("minishell, version 1.0-release-(x86_64-pc-linux-gnu)\n"
	"Usage:\tminishell [GNU long option] [option] ...\n"
	"\tminishell [GNU long option] [option] script-file ...\n"
	"GNU long options:\n"
	"\t--bash-compliant\n"
	"\t--help\n"
	"\t--init-file\n"
	"\t--login\n"
	"\t--noprofile\n"
	"\t--norc\n"
	"\t--rcfile\n"
	"\t--verbose\n"
	"\t--version\n"
	"Shell options:\n"
	"\t-clv\n");
	return 0;
}
