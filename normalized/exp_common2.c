/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_common2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:37:30 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 00:37:31 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/* Checks if the character after $ is a special shell variable.*/
bool	extract_special_var(char *dlr_varname, char c, size_t *i, size_t k)
{
	if (c == '$' || c == '?' || c == '#' || c == '*')
	{
		dlr_varname[0] = c;
		dlr_varname[1] = '\0';
		*i = k + 1;
		return (true);
	}
	return (false);
}
