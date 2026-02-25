/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_parser4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 16:01:46 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 16:04:39 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt_parser.h"

bool	its_logical_and(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '&' && prompt[pi + 1] == '&')
		return (true);
	return (false);
}

bool	its_logical_or(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '|' && prompt[pi + 1] == '|')
		return (true);
	return (false);
}

bool	its_pipe(char *prompt, size_t plen, size_t pi)
{
	if (pi + 1 < plen && prompt[pi] == '|' && prompt[pi + 1] != '|')
		return (true);
	return (false);
}

void	skip_spaces(char *prompt, size_t *pi)
{
	while (prompt[*pi] == ' ' && *pi < ft_strlen(prompt))
		++(*pi);
}
