#include "shell.h"

// HOSTTYPE		
int	set_local_hosttype(t_env *env)
{
	env->vars[SL_HOSTTYPE].name = ft_strdup("HOSTTYPE");
	env->vars[SL_HOSTTYPE].value = ft_strdup(MSH_ARCH);
	return (COMMON_SUCCESS);
}

// Let's consider get_next_line() error as system error
char	*get_hostname(void)
{
	int		fd;			// Host file descriptor
	int		gnlerr;		// get_next_line() error
	char	*hostname;

	fd = open(HOSTNAME_PATH, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (NULL);
	}
	hostname = get_next_line(fd, &gnlerr);
	if (!hostname && gnlerr)
	{	
		print_shell_error("get_next_line()", GNL_ERR_MSG);
		gnl_finish(fd); // Obligatory!
		return (NULL);
	}
	if (close(fd) == -1)
	{
		perror("close");
		return (NULL);
	}
	if (hostname[ft_strlen(hostname) - 1] == '\n')
		hostname[ft_strlen(hostname) - 1] = '\0';
	return (COMMON_SUCCESS);
}

// HOSTNAME	
int	set_local_hostname(t_env *env)
{
	char	*hostname;

	hostname = get_hostname();
	if (!hostname)
		return (COMMON_SYS_ERR);
	env->vars[SL_HOSTNAME].name = ft_strdup("HOSTNAME");
	env->vars[SL_HOSTNAME].value = ft_strdup(hostname);
	return (COMMON_SUCCESS);
}

// OSTYPE
int	set_local_ostype(t_env *env)
{
	env->vars[SL_OSTYPE].name = ft_strdup("OSTYPE");
	env->vars[SL_OSTYPE].value = ft_strdup(MSH_OSTYPE);
	return (COMMON_SUCCESS);
}

// HOSTMACH
int	set_local_hostmach(t_env *env)
{
	char	*machtype;
	size_t	mt_len;

	mt_len = ft_strlen(MSH_ARCH) + 4 + ft_strlen(MSH_OSTYPE) + 1;
	machtype = malloc(mt_len * sizeof(char));
	ft_strlcpy(machtype, MSH_ARCH, mt_len);
	ft_strlcat(machtype, "-pc-", mt_len);
	ft_strlcat(machtype, MSH_OSTYPE, mt_len);
	env->vars[SL_MACHTYPE].name = ft_strdup("MACHTYPE");
	env->vars[SL_MACHTYPE].value = ft_strdup(machtype);
	return (COMMON_SUCCESS);
}
