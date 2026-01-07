/* gcc test_2.c libvector.a -O0 -g3 -o test_2 */
#include "vector.h"

int	main()
{
	t_vector	v;
	int			i;
	int			var;

	vector_init(&v, INT, 10);

	i = 0;
	while (i < 10)
	{
		vector_push_back(&v, (void *)&i);
		++i;
	}

	printf("Vector: "); vector_print(&v, " "); printf("\n\n");

	var = 100;
	vector_push_back(&v, (void *)&var);

	var = 200;
	vector_push_back(&v, (void *)&var);

	printf("Vector: "); vector_print(&v, " "); printf("\n");

	vector_free(&v);
	return (0);
}
