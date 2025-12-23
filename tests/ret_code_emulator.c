#include <stdio.h>

int	main(int argc, char **argv)
{
	switch (argc)
	{
		case 1: return 0;
		case 2: return 1;
		case 3: return 2;
		case 4: return 3;
	}
	return 0;
}
