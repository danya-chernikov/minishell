#include "vector.h"

/* Changes current vector capacity */
int	vector_reserve(t_vector *v, size_t new_cap)
{
	return (1);
}

/* Sets new enlarger */
void	vector_set_enlarger(t_vector *v, size_t enlarger)
{
	v->enlarger = enlarger;
}

void	vector_free(t_vector *v)
{
	free(v->data);
}
