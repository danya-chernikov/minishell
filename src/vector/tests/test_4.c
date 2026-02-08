#include "vector.h"

#include <stdio.h>
#include <string.h>

void	func(t_vector *v)
{
	for (int i = 21; i < 40; ++i)
	{
		char	tmp = i + '0';
		vector_push_back(v, (void *)&tmp);
	}
}

int	main()
{
	char huge_buf[] = "AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDD"
					  "EEEEEEEEFFFFFFFFGGGGGGGGHHHHHHHH"
					  "IIIIIIIIJJJJJJJJKKKKKKKKLLLLLLLL"
					  "MMMMMMMMNNNNNNNNOOOOOOOOPPPPPPPP"
					  "QQQQQQQQRRRRRRRRSSSSSSSSTTTTTTTT"
					  "UUUUUUUUVVVVVVVVWWWWWWWWXXXXXXXX"
					  "YYYYYYYYZZZZZZZZ1111111122222222";

	t_vector v;
	t_vector v2;

	vector_init(&v, CHAR, 10);

	for (int i = 0; i < strlen(huge_buf); ++i)
		vector_push_back(&v, (void *)&huge_buf[i]);

	printf("Vector: "); vector_print(&v, ""); printf("\n");

	vector_free(&v);

	vector_init(&v2, CHAR, 10);

	for (int i = 0; i < 20; ++i)
	{
		char	tmp = i + '0';
		vector_push_back(&v2, (void *)&tmp);
	}

	printf("Vector: "); vector_print(&v2, ""); printf("\n");

	func(&v2);

	printf("Vector: "); vector_print(&v2, ""); printf("\n");

	vector_free(&v2);

	return 0;
}
