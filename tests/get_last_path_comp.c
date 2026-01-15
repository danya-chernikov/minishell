#include <stdio.h>
#include <string.h>

void    get_last_path_comp(char *path)
{
    int i;
    int j;
    int slash_ind;
    size_t  path_len;
    
    i = 0;
    slash_ind = -1;
    path_len = (int)strlen(path);
    while (i < path_len)
    {   
        if (path[i] == '/')
            slash_ind = i;
        ++i;
    }
    if (slash_ind == -1)
        return ;
    j = 0;
    i = slash_ind + 1;
    while (i < path_len)
    {   
        path[j] = path[i];
        ++i;
        ++j;
    }
    path[j] = '\0';
}

int main()
{
    char *path = "/home/dchernik/Downloads/circle03/minishell/src/minishell";
    get_last_path_comp(path);
    printf("%s\n", path);
    return 0;
}
