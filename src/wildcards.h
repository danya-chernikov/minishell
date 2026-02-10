#ifndef WILDCARDS_H
# define WILDCARDS_H

# define MAX_WC_MASK_LEN	4096
# define MAX_WC_FILE_NUM	8192

/* wildcards.c */
int		expand_wildcards(char *mask);
int		wc_get_curdir_files(char *files[], size_t *file_cnt);
int		wc_init_dir(DIR **dir, struct dirent **entry, char *cwd);
int		wc_get_curdir_files_loop(DIR *dir, struct dirent *entry, char *files[], size_t *file_cnt);

/* wildcards2.c */
void	wc_sift_files_by_mask(char *files[], size_t file_cnt, char *mask);
void	wc_check_file(char **res_files, char *mask, char *file, size_t *resf_cnt);
void	wc_check_file_loop(char *file, char *mask, bool *f_cancel, size_t *mi);

#endif
