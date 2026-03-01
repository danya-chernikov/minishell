/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:17:58 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/27 15:13:43 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int		ret_code;

	ret_code = EXIT_SUCCESS;
	fret = msh_init(&msh, argc, argv, env);
	if (fret != COMMON_SUCCESS)
	{
		ret_code = EXIT_FAILURE;
		if (fret == MUST_EXIT)
			ret_code = EXIT_SUCCESS;
		msh_free(&msh);
		return (ret_code);
	}
	fret = msh_launch(&msh, &ret_code);
	if (fret == COMMON_SYS_ERR)
		ret_code = EXIT_FAILURE;
	msh_free(&msh);
	return (ret_code);
}
