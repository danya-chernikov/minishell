/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:54:23 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/25 11:54:27 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#include <stdlib.h>

void	configs_init(t_configs *cnf)
{
	cnf->etc_prof_path = NULL;
	cnf->home_prof_path = NULL;
	cnf->home_login_path = NULL;
	cnf->home_cmn_prof_path = NULL;
	cnf->home_logout_path = NULL;
	cnf->etc_logout_path = NULL;
	cnf->etc_rc_path = NULL;
	cnf->home_rc_path = NULL;
}

void	configs_free(t_configs *cnf)
{
	if (cnf->etc_prof_path)
		free(cnf->etc_prof_path);
	if (cnf->home_prof_path)
		free(cnf->home_prof_path);
	if (cnf->home_login_path)
		free(cnf->home_login_path);
	if (cnf->home_cmn_prof_path)
		free(cnf->home_cmn_prof_path);
	if (cnf->home_logout_path)
		free(cnf->home_logout_path);
	if (cnf->etc_logout_path)
		free(cnf->etc_logout_path);
	if (cnf->etc_rc_path)
		free(cnf->etc_rc_path);
	if (cnf->home_rc_path)
		free(cnf->home_rc_path);
}
