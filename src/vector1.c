#include "vector.h"

/* Yeah, I know we could specify the vector's data type size
 * by passing sizeof() directly as the second argument in the
 * caller, but I think the approach we're using at the moment
 * could be useful in the future */
int	vector_init(t_vector *v, t_data_type dtype, size_t cap)
{
	v->enlarger = DEFAULT_VEC_ENLARGER;
	v->data_type = dtype;
	set_cap1(v);
	set_cap2(v);
	v->data = malloc(v->bsize);
	if (!v->data)
		return 0;
	return 1;
}

// Fills all elements with `filler`
int	vector_init(t_vector *v, t_data_type dtype, size_t cap, void *filler)
{	
	v->enlarger = DEFAULT_VEC_ENLARGER;
	v->data_type = dtype;
	if (dtype == )
	v->capacity = sizeof (dtype) * cap;
	v->data = malloc(sizeof (dtype) * cap);
	if (!v->data)
		return 0;
	ft_memset(v->data, filler, );
	return 1;
}

// Changes current vector capacity
int		vector_reserve(t_vector *v, size_t new_cap)
{

}

// Sets new enlarger
void	vector_set_enlarger(t_vector *v, size_t enlarger)
{
	v->enlarger = enlarger;
}

void	vector_free(t_vector *v)
{
	free(v->data);
}
