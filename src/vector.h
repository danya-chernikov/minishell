#ifndef VECTOR_H
# define VECTOR_H

/* Basic slow and UNSAFE vector implementation
 * When we call vector_init() and pass it a capacity, it creates
 * a vector with that capacity. When adding new elements, if the
 * vector is about to overflow, it increases its current capacity
 * by DEFAULT_VEC_ENLARGER or by the value specified in the optional
 * third argument, enlarger, when overloading vector_init() */

# include "aux.h"

# define DEFAULT_VEC_ENLARGER	128

typedef enum e_data_type
{
	CHAR,
	U_CHAR,
	SHORT,
	U_SHORT,
	INT,
	U_INT,
	LONG,
	U_LONG,
	LONG_LONG,
	U_LONG_LONG,
	FLOAT,
	DOUBLE,
	LONG_DOUBLE,
	VOID
}	t_data_type;

typedef struct s_vector
{
	size_t		capacity;
	size_t		enlarger;
	size_t		size;
	size_t		bsize;		// Size of vector in bytes
	t_data_type	data_type;
	void		*data;
	void		*front;		// The first element
	void		*back;		// The last element
}	t_vector;

/* vector1.c */
/* Functions for creating, removing vector, and changing its parameters */
int		vector_init(t_vector *v, t_data_type dtype, size_t cap);				// Fills nothing, just leaves memory data
int		vector_init(t_vector *v, t_data_type dtype, size_t cap, void *filler);	// Fills all elements with `filler`
int		vector_reserve(t_vector *v, size_t new_cap);							// Changes current vector capacity
void	vector_set_enlarger(t_vector *v, size_t enlarger);						// Sets new enlarger
void	vector_free(t_vector *v);

/* vector2.c */
/* Functions for accessing vector's elements. Does not change the vector */
void	*vector_at(const t_vector *v, size_t ind);				// Accesses the element at index `ind`
void	*vector_mat(const t_vector *v, size_t ri, size_t ci);	// Accesses the element at index v->data[ri][ci]
void	*vector_front(const t_vector *v);						// Access the first element
void	*vector_back(const t_vector *v);						// The last

/* vector3.c */
/* Functions for returning vector's info */
bool	vector_empty(t_vector *v);
size_t	vector_size(t_vector *v);
size_t	vector_strlen(t_vector *v);	// Treats vector as a string and returns this string's length
size_t	vector_capacity(t_vector *v);

/* vector4.c */
/* Functions that change the vector's content */
void	vector_si(t_vector *v, size_t ind);				// Change the value of the vector's cell at index `ind`, i.e. v->data[ind]
void	vector_msi(t_vector *v, size_t ri, size_t ci);	// The same as vector_si() but for matrix
void	vector_push_back(t_vector *v, void *data);
void	*vector_pop_back(t_vector *v);

/* vector_service.c
 * Service functions */
static void set_bsize1(t_vector *v);
static void set_bsize2(t_vector *v);

#endif
