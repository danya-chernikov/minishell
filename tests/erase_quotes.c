#include <stdio.h>
#include <string.h>

int erase_quotes(char *str)
{
    size_t  i;
    
    if ((str[0] == '\'' && str[strlen(str) - 1] != '\'') ||
        (str[0] == '"' && str[strlen(str) - 1] != '"'))
    {   
        return (0);
    }
	if ((str[strlen(str) - 1] == '\'' && str[0] != '\'') ||
		(str[strlen(str) - 1] == '"' && str[0] != '"'))
	{
		return (0);
	}
    if (str[0] == '\'' || str[0] == '"')
    {
        str[strlen(str) - 1] = '\0';
        i = 0;
        while (i < strlen(str) - 1)
        {   
            str[i] = str[i + 1];
            ++i;
        }
    }
    return (1);
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (erase_quotes(argv[i]))
            printf("%s\n", argv[i]);
        else
            printf("Syntax error\n");
    }
    return 0;
}
