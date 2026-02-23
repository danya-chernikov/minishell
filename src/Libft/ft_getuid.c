#include "libft.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int  parse_first_id_from_status(const char *key, uid_t *out);

int ft_getuid(uid_t *uid)
{
    if (!uid)
        return (-1);
    return (parse_first_id_from_status("Uid:", uid));
}

static int  parse_first_id_from_status(const char *key, uid_t *out)
{
    int     fd;
    char    *line;
    int     err;
	char	*p;

    fd = open("/proc/self/status", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return (-1);
    }
    err = 0;
    line = get_next_line(fd, &err);
    while (line)
    {
        if (!ft_strncmp(line, key, ft_strlen(key)))
        {
            p = line + ft_strlen(key);
            while (*p == ' ' || *p == '\t')
                ++p;
            *out = (uid_t)ft_atoi(p);
            free(line);
            gnl_finish(fd);
            close(fd);
            return (0);
        }
        free(line);
        line = get_next_line(fd, &err);
    }
    if (err)
        perror("get_next_line");
    gnl_finish(fd);
    close(fd);
    return (-1);
}
