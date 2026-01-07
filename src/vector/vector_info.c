#include "vector.h"

bool	vector_empty(const t_vector *v)
{
	return (v->size);
}

size_t	vector_size(const t_vector *v)
{
	return (v->size);
}

size_t	vector_strlen(const t_vector *v)
{
	size_t	len;
	t_uchar	*data;

	len = 0;
	data = (t_uchar *)v->data;
	while (data[len] != '\0')
		++len;
	return (len);
}

size_t	vector_capacity(const t_vector *v)
{
	return (v->capacity);
}
