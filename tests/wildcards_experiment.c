#include <stdio.h>
#include <string.h>
#include <stdbool.h>

# define FN		10
# define FLEN	30
 
int main(int argc, char **argv)
{
    char mask[] = "a*b*c";
    char files[FN][FLEN] = {
        "gui",
        "a",
        "auuubwvcoo",
        "abc",
        "abryruzcpp",
        "aaabxxxc",
        "a.bc",
        "ab.c",
        "aqbce",
        "i"
    };

    bool f_cancel;

    for (int fi = 0; fi < FN; ++fi)
    {
        f_cancel = false;
        for (int mi = 0; mi < strlen(mask); ++mi)
        {
			int	ci;
            for (ci = 0; ci < strlen(files[fi]); ++ci)
            {
                if (mask[mi] == '*')
                    continue;
                else
                {
                    if (files[fi][ci] != mask[mi])
                    {
                        f_cancel = true;
                        break;
                    }
                }
            }
			if (ci == strlen(files[fi] && f_cancel)
				break;
            if (f_cancel)
                break;
        }
        if (!f_cancel)
            printf("%s\n", files[fi]);
    }

    return 0;
}
