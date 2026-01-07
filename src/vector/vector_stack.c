#include "vector.h"

int	vector_push_back(t_vector *v, void *val)
{
	if (v->size + 1 > v->capacity)
	{			
		v->capacity += v->enlarger;
		v->data = vector_realloc(v->data, v->bcapacity, v->capacity * v->esize);
		if (!v->data)
			return (0);
		v->bcapacity = v->capacity * v->esize;
	}

	if (v->data_type == CHAR)
		((char *)v->data)[v->size] = ((char *)val)[0];
	else if (v->data_type == U_CHAR)
		((t_uchar *)v->data)[v->size] = ((t_uchar *)val)[0];
	else if (v->data_type == INT)
		((int *)v->data)[v->size] = *((int *)val);

	++v->size;
	v->front = v->data;
	v->back = v->data + (v->size - 1) * v->esize;
	return (1);
}

/* On error returns NULL */
void	*vector_pop_back(t_vector *v)
{
	void	*ret;

	if (v->size == 0)
		return (NULL);
	ret = v->data + ((v->size - 1) * v->esize);
	--v->size;
	v->back = v->data + (v->size - 1) * v->esize;
	return (ret);
}
