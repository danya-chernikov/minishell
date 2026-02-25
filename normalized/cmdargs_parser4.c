/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdargs_parser4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 04:54:32 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 04:54:33 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdargs_parser.h"

#include "error.h"
#include "libft.h"

bool	is_c_opt(char *opt)
{
	if (!ft_strncmp(opt, C_SHORT_OPT, ft_strlen(C_SHORT_OPT)))
		return (true);
	return (false);
}

void	print_help(void)
{
	printf("%s, version %s-(%s-pc-%s)\n",
		MSH_NAME_LONG, MSH_VERSION, MSH_ARCH, MSH_OSTYPE);
	printf("Usage:\t%s [GNU long option] [option] ...\n", MSH_NAME_LONG);
	printf("\t%s [GNU long option] [option] script-file ...\n", MSH_NAME_LONG);
	printf("GNU long options:\n"
		"\t--help\n"
		"\t--login\n"
		"\t--norc\n"
		"\t--verbose\n"
		"\t--version\n"
		"Shell options:\n"
		"\t-clv\n");
}

void	print_version(void)
{
	printf("%s, version %s (%s-pc-%s)",
		MSH_NAME_LONG, MSH_VERSION, MSH_ARCH, MSH_OSTYPE);
	printf("Copyleft (C) 2026 Urduliz 42\n");
	printf("License 42 Educational: <https://42.fr>\n");
	printf("\nThis is free software; you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
}
