#include "wildcards.h"
#include "operand.h"

#include "libft.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

/* Expands wildcards using mask.
 *     res_files	- the result of globbing;
 *     files		- initial files in current directory
 * */
int	expand_wildcards(char **res_files, char *mask)
{
	char	*files[MAX_FILE_NUM];
	size_t	file_cnt;
	int		fret;
	int		i;

	fret = wc_get_curdir_files(files, &file_cnt);
	if (fret != COMMON_SUCCESS)
		return (fret);
	wc_sift_files_by_mask(files, file_cnt, mask);
	i = 0;
	while (i < file_cnt)
		free(files[i++]);
	return (COMMON_SUCCESS);
}

/* Fills `files` with file names of the current directory.
 *     i	- auxiliary counter;
 *     cwd	- current work directory.
 * THINK ABOUT HOW TO AVOID USING errno HERE! */
int	wc_get_curdir_files(char *files[], size_t *file_cnt)
{
	DIR				*dir;
	struct dirent	*entry;
	char			cwd[PATH_MAX];

	if (wc_init_dir(&dir, &entry, &cwd) != COMMON_SUCCESS)
		return (COMMON_SYS_ERR);
	if (wc_get_curdir_files_loop(dir, entry, files, file_cnt) != COMMON_SUCCESS)
	{
		perror("malloc/readdir");
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

int	wc_init_dir(DIR **dir, struct dirent **entry, char *cwd)
{
	if (!getcwd(cwd, PATH_MAX))
	{
		perror("getcwd");
		return (COMMON_SYS_ERR);
	}
	*dir = opendir(cwd);
	if (!(*dir))
	{
		perror("opendir");
		return (COMMON_SYS_ERR);
	}
	errno = 0;
	*entry = readdir(*dir);
	if (errno && !(*entry))
	{
		perror("readdir");
		closedir(*dir):
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

int	wc_get_curdir_files_loop(DIR *dir, struct dirent *entry, char *files[], size_t *file_cnt)
{
	int	i;

	*file_cnt = 0;
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			files[*file_cnt] = (char *)malloc((ft_strlen(entry->d_name) + 1) * sizeof(char));
			if (!files[*file_cnt])
			{
				i = 0;
				while (i < *file_cnt)
					free(files[i++]);
				return (COMMON_SYS_ERR);
			}
			ft_strlcpy(files[*file_cnt], entry->d_name, ft_strlen(entry->d_name));
			++(*file_cnt);
		}
		entry = readdir(dir);
	}
	if (errno && !entry)
		return (COMMON_SYS_ERR);
	return (COMMON_SUCCESS);
}
