#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

/* Rewrite it using vector! */
 
# define MAX_FILE_NUM	128
# define FLEN			30
 
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <mask>\n", argv[0]);
		return 1;
	}

    char			*files[MAX_FILE_NUM];
    char			*mask;
	size_t			file_cnt;
	int				i;				// Auxiliary counter

	DIR				*dir;
	struct dirent	*entry;
	char			cwd[PATH_MAX];	// Current work directory
 
    bool			f_cancel;		// If true we'll not print the current file
	char			*file;			// Pointer to the current file
	size_t			mi;				// Mask index

	file_cnt = 0;
	if (!getcwd(cwd, PATH_MAX))
	{
		perror("getcwd");
		return 1;
	}
	dir = opendir(cwd);
	if (!dir)
	{
		perror("opendir");
		return 1;
	}
	errno = 0;
	entry = readdir(dir);
	if (errno && !entry)
	{
		perror("readdir");
		return 1;
	}
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			files[file_cnt] = malloc((strlen(entry->d_name) + 1) * sizeof(char));
			if (!files[file_cnt])
			{
				i = 0;
				while (i < file_cnt)
					free(files[i++]);
				perror("malloc");
				return 1;
			}
			strncpy(files[file_cnt], entry->d_name, strlen(entry->d_name));
			++file_cnt;
		}
		entry = readdir(dir);
	}
	if (errno && !entry)
	{
		perror("readdir");
		return 1;
	}

	printf("Files of current directory:\n");
	i = 0;
	while (i < file_cnt)
	{
		printf("%s\n", files[i]);
		++i;
	}
	printf("\n");

	printf("Files after globbing:\n");
	mask = argv[1];
    for (size_t fi = 0; fi < file_cnt; ++fi)
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
				break ;
			}
			if (mask[mi] == '*')
			{
				if (mi + 1 == strlen(mask)) // The last asterix
				{
					// Any number of any symbols that go
					// after current ci index is suitable
					++mi;
					break ;
				}
				// Skip all symbols in the file
				// until find the symbol that is
				// equal to the next mask symbol
				while (ci < strlen(file))
				{
					if (file[ci] == mask[mi + 1])
					{
						++mi; // Go to the next mask symbol
						break ;
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
					break ;
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
            
    } //for (int fi = 0; fi < file_cnt; ++fi)
	
	i = 0;
	while (i < file_cnt)
		free(files[i++]);
	
    return 0;
}
