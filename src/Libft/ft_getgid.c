#include "libft.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int  parse_first_gid_from_status(gid_t *out);

int ft_getgid(gid_t *gid)
{
    if (!gid)
        return (-1);
    return (parse_first_gid_from_status(gid));
}

static int  parse_first_gid_from_status(gid_t *out)
{
    int     fd;
    char    *line;
    int     err;

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
        if (!ft_strncmp(line, "Gid:", 4))
        {
            char *p = line + 4;
            while (*p == ' ' || *p == '\t')
                ++p;
            *out = (gid_t)ft_atoi(p);
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
