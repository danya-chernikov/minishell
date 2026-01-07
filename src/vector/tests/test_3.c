/* gcc test_3.c libvector.a -O0 -g3 -o test_3 */
#include "vector.h"
#include <errno.h>

typedef struct s_coords
{
	int	x;
	int	y;
}	t_coords;

int	main()
{
	t_vector	v;
	int			rand_val;
	int			i;
	int			var;
	FILE		*fp;

	int	(*vpb)(t_vector *, void *);

	vpb = vector_push_back;

	vector_init(&v, VOID, 1);

	fp = fopen("/dev/random", "r");
	if (!fp)
		perror("fopen");
	i = 0;
	while (i < 15)
	{
		t_coords	tmp;

		fread(&rand_val, sizeof(rand_val), 1, fp);
		tmp.x = rand_val % 100;
		fread(&rand_val, sizeof(rand_val), 1, fp);
		tmp.y = rand_val % 100;
		printf("tmp.x = %d ; tmp.y = %d\n", tmp.x, tmp.y);
		vector_push_back(&v, (void *)&tmp);
		++i;
	}
	if (fclose(fp) == EOF)
		perror("fclose");

	printf("vector_size(&v) = %zu\n", vector_size(&v));
	printf("Vector:\n"); vector_print(&v, "\n"); printf("\n\n");

	i = 0;
	while (i < vector_size(&v))
	{
		printf("%d\t%d\t%d\n",
			i + 1,
			((t_coords *)vector_at(&v, i))->x,
			((t_coords *)vector_at(&v, i))->y);
		++i;
	}

	vector_free(&v);
	return (0);
}
