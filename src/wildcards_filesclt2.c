#include "wildcards.h"

#include "libft.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int	wc_add_curdir_file(DIR *dir, struct dirent *entry, char *files[], size_t *file_cnt)
{
	size_t	i;
	size_t	dname_len;

	dname_len = ft_strlen(entry->d_name);
	files[*file_cnt] = (char *)malloc((dname_len + 1) * sizeof(char));
	if (!files[*file_cnt])
	{
		i = 0;
		while (i < *file_cnt)
			free(files[i++]);
		closedir(dir);
		return (COMMON_SYS_ERR);
	}
	if (*file_cnt >= WC_MAX_FILES_NUM - 1)
	{
		print_shell_error(NULL, MAX_WC_FNUM_ERR_MSG);
		return (COMMON_FAILURE);
	}
	ft_strlcpy(files[*file_cnt], entry->d_name, dname_len + 1);
	++(*file_cnt);
	return (COMMON_SUCCESS);
}

void	wc_free_curdir_files(char *files[], size_t file_cnt)
{
	size_t	i;

	i = 0;
	while (i < file_cnt)
		free(files[i++]);
}
