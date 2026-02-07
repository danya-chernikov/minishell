#include "vector.h"

#include <stdio.h>
#include <string.h>

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

	vector_init(&v, CHAR, 10);

	for (int i = 0; i < strlen(huge_buf); ++i)
	{
		vector_push_back(&v, (void *)&huge_buf[i]);
	}

	printf("Vector: "); vector_print(&v, ""); printf("\n");

	vector_free(&v);

	return 0;
}
