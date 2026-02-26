/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:50:29 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/26 01:17:18 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

# include "prompt_parser.h"

/* engine.c */
int	shell_engine(t_shell *msh, char *prompt, int *ret_code);
int	run_parser(t_shell *msh, t_parser_data *pdata, int *ret_code);
int	comments_parser(t_parser_data *d);
int	operands_quotes_parser(t_parser_data *d);

#endif
