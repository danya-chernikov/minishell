/* gcc test_1.c libvector.a -O0 -g3 -o test_1 */
#include "vector.h"

int	main()
{
	t_vector	v;
	int			i;
	int			var;
	int			last;
	int			first;

	vector_init(&v, INT, 10);

	var = 10;
	vector_push_back(&v, (void *)&var);
	printf("Vector: "); vector_print(&v, " "); printf("\n");
	printf("vector_front() = %d\n", *((int *)vector_front(&v)));
	printf("vector_back() = %d\n", *((int *)vector_back(&v)));
	printf("\n");

	var = 20;
	vector_push_back(&v, (void *)&var);
	printf("Vector: "); vector_print(&v, " "); printf("\n");
	printf("vector_front() = %d\n", *((int *)vector_front(&v)));
	printf("vector_back() = %d\n", *((int *)vector_back(&v)));
	printf("\n");

	last = *((int *)vector_pop_back(&v));
	printf("\nlast = %d\n", last);
	printf("Vector: "); vector_print(&v, " "); printf("\n");

	last = *((int *)vector_pop_back(&v));
	printf("\nlast = %d\n", last);
	printf("Vector: "); vector_print(&v, " "); printf("\n");

	vector_free(&v);
	return (0);
}
