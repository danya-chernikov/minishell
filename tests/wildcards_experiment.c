#include <stdio.h>
#include <string.h>
#include <stdbool.h>
 
# define FN        10
# define FLEN    30
 
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
 
    bool	f_cancel;	// If true we'll not print the current file
	char	*file;		// Pointer to the current file
	size_t	mi;			// Mask index

    for (size_t fi = 0; fi < FN; ++fi)
    {
		file		= files[fi];
        f_cancel	= false;
		mi			= 0;

		for (size_t ci = 0; ci < strlen(file); ++ci)
		{
			if (mi == strlen(mask))
			{
				if (ci < strlen(file))
					f_cancel = true;
				break;
			}
			if (mask[mi] == '*')
			{
				if (mi + 1 == strlen(mask)) // The last asterix
				{
					// Any number of any symbols that go
					// after current ci index is suitable
					break;
				}
				// Skip all symbols in the file
				// until find the symbol that is
				// equal to the next mask symbol
				while (ci < strlen(file))
				{
					if (file[ci] == mask[mi + 1])
					{
						++mi; // Go to the next mask symbol
						break;
					}
					++ci;
				}
				--ci;
			}
			else // if (mask[mi] != '*')
			{
				if (file[ci] != mask[mi])
				{
					f_cancel = true;
					break;
				}
				else // Go to the next mask symbol
					++mi;
			}
		} // for (int ci = 0; ci < strlen(files[fi]); ++ci)
		
		// If mask symbols were not completely traversed
		if (mi < strlen(mask))
			f_cancel = true;

        if (!f_cancel)
            printf("FILE: %s\n", file);
            
    } //for (int fi = 0; fi < FN; ++fi)
 
    return 0;
}
