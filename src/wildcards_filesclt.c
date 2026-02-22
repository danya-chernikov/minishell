#include "wildcards.h"

#include "libft.h"
#include "vector.h"
#include "error.h"

#include <stdio.h>

#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

/* Fills `files` with file names of the current directory.
 *     i	- auxiliary counter;
 *     cwd	- current work directory.
 * THINK ABOUT HOW TO AVOID USING errno HERE! */
int	wc_get_curdir_files(char *files[], size_t *file_cnt)
{
	DIR				*dir;
	struct dirent	*entry;
	char			cwd[PATH_MAX];

	if (wc_init_dir(&dir, &entry, cwd) != COMMON_SUCCESS)
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
		closedir(*dir);
		return (COMMON_SYS_ERR);
	}
	return (COMMON_SUCCESS);
}

/* Traverses all files in the current directory stores their names into `files` */
int	wc_get_curdir_files_loop(DIR *dir, struct dirent *entry, char *files[], size_t *file_cnt)
{
	int	fret;

	*file_cnt = 0;
	fret = COMMON_SUCCESS;
	while (entry != NULL)
	{
		if (ft_strlen(entry->d_name) >= WC_MAX_FILENAME_LEN - 1)
		{
			print_shell_error(NULL, MAX_WC_FN_LEN_ERR_MSG);
			return (COMMON_FAILURE);
		}
		if (entry->d_name[0] != '.')
		{
			fret = wc_add_curdir_file(dir, entry, files, file_cnt);
			if (fret != COMMON_SUCCESS)
				return (fret);
		}
		entry = readdir(dir);
	}
	if (errno && !entry)
		fret = COMMON_SYS_ERR;
	closedir(dir);
	return (fret);
}
