#ifndef WILDCARDS_H
# define WILDCARDS_H

# define WC_MAX_MASK_LEN		4096
# define WC_MAX_FILES_NUM		8192
# define WC_MAX_FILENAME_LEN	4096
# define WC_BREAK				6

# include <stddef.h>
# include <stdbool.h>

#include <dirent.h>
#include <sys/types.h>

typedef struct s_vector	t_vector;

/* To pass the Norm.
 *     file		- current file we're checking;
 *     mask		- wildcards mask;
 *     qmask	- quotes mask;
 *     mi		- mask index;
 *     ci		- character index in `file` */
typedef struct s_wc_data
{
	char		*file;
	size_t		file_len;
	char		*mask;
	size_t		mask_len;
	t_vector	*qmask;
	size_t		mi;
	size_t		ci;

}	t_wc_data;

/* wildcards_filesclt.c */
int		expand_wildcards(char **res_files, char *mask, t_vector *qmask);
int		wc_get_curdir_files(char *files[], size_t *file_cnt);
int		wc_init_dir(DIR **dir, struct dirent **entry, char *cwd);
int		wc_get_curdir_files_loop(DIR *dir, struct dirent *entry, char *files[], size_t *file_cnt);

/* wildcards_filesclt2.c */
int		wc_add_curdir_file(DIR *dir, struct dirent *entry, char *files[], size_t *file_cnt);
void	wc_free_curdir_files(char *files[], size_t file_cnt);

/* wildcards_sieve.c */
int		wc_sift_files_by_mask(t_wc_data *wcd, char **res_files, char *files[], size_t file_cnt);
int		wc_check_file(t_wc_data	*wcd, char **res_files, size_t *resf_cnt);
void	wc_check_file_loop(t_wc_data *wcd, bool *f_cancel);
int		wc_match_asterisk(t_wc_data *wcd);
int		wc_match_regular_symbol(t_wc_data *wcd, bool *f_cancel);

/* wildcards_sieve2.c */
int		wc_check_zero_matches(t_wc_data *wcd, char **res_files, size_t *resf_cnt);

/* wildcards_common.c */
bool	wc_is_asterisk(char expres_char, char qmask_char);
void	wc_trim_vectors(t_vector *exp_res, t_vector *qmask, size_t new_len);
void	wc_collapse_conseq_asterisks(t_vector *exp_res, t_vector *qmask);
int		wc_alloc_res(char ***wc_res);
void	wc_free_res(char ***wc_res);

#endif
