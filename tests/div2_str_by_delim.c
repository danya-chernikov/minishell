#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ENV_NAME_LEN    128
#define MAX_ENV_VAL_LEN     128

int div2_str_by_delim(char *str, char delim, char **part1, char **part2)
{   
    size_t  i; 
    
    if (strlen(str) - 1 > MAX_ENV_NAME_LEN + MAX_ENV_VAL_LEN)
        return (0);
    *part1 = (char *)malloc(MAX_ENV_NAME_LEN * sizeof(char));
    if (!(*part1))
    {   
        perror("malloc");
        return (-1);
    }
    *part2 = (char *)malloc(MAX_ENV_VAL_LEN * sizeof(char));
    if (!(*part2))
    {   
        perror("malloc");
        free(*part1);
        return (-1);
    }
    i = 0;
    while (str[i] && str[i] != delim)
	{
        (*part1)[i] = str[i];
		++i;
	}
    (*part1)[i] = '\0';
	++i;
    while (str[i])
	{
        (*part2)[i - strlen(*part1) - 1] = str[i];
		++i;
	}
    (*part2)[i - strlen(*part1) - 1] = '\0';
    return (1);
}

int main()
{
    char str[] = "VAR=VALUE";
    char *name;
    char *value;
    
    name = NULL;
    value = NULL;
    if (div2_str_by_delim(str, '=', &name, &value) == 1)
    {
        printf("name: %s\nvalue: %s\n", name, value);
    }
    return 0;
}
