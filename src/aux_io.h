/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_io.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:50:08 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 22:12:14 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUX_IO_H
# define AUX_IO_H

# include <sys/types.h>

int	write_all(int fd, const char *buf, size_t n);

#endif
