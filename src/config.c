/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:54:23 by dchernik          #+#    #+#             */
/*   Updated: 2026/02/28 02:22:35 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#include <stdlib.h>

static void	configs_free_first_part(t_configs *cnf);
static void	configs_free_second_part(t_configs *cnf);

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
	configs_free_first_part(cnf);
	configs_free_second_part(cnf);
}

static void	configs_free_first_part(t_configs *cnf)
{
	if (cnf->etc_prof_path)
	{
		free(cnf->etc_prof_path);
		cnf->etc_prof_path = NULL;
	}
	if (cnf->home_prof_path)
	{
		free(cnf->home_prof_path);
		cnf->home_prof_path = NULL;
	}
	if (cnf->home_login_path)
	{
		free(cnf->home_login_path);
		cnf->home_login_path = NULL;
	}
	if (cnf->home_cmn_prof_path)
	{
		free(cnf->home_cmn_prof_path);
		cnf->home_cmn_prof_path = NULL;
	}
}

static void	configs_free_second_part(t_configs *cnf)
{
	if (cnf->home_logout_path)
	{
		free(cnf->home_logout_path);
		cnf->home_logout_path = NULL;
	}
	if (cnf->etc_logout_path)
	{
		free(cnf->etc_logout_path);
		cnf->etc_logout_path = NULL;
	}
	if (cnf->etc_rc_path)
	{
		free(cnf->etc_rc_path);
		cnf->etc_rc_path = NULL;
	}
	if (cnf->home_rc_path)
	{
		free(cnf->home_rc_path);
		cnf->home_rc_path = NULL;
	}
}
