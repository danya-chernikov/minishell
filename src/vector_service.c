#include "vector.h"

static void set_bsize1(t_vector *v)
{
	if (v->dtype == CHAR)
		v->bsize = v->capacity * sizeof (char);
	else if (v->dtype == U_CHAR)
		v->bsize = v->capacity * sizeof (t_uchar);
	else if (v->dtype == SHORT)
		v->bsize = v->capacity * sizeof (short);
	else if (v->dtype == U_SHORT)
		v->bsize = v->capacity * sizeof (t_ushort);
	else if (v->dtype == INT)
		v->bsize = v->capacity * sizeof (int);
	else if (v->dtype == U_INT)
		v->bsize = v->capacity * sizeof (t_uint);
	else if (v->dtype == LONG)
		v->bsize = v->capacity * sizeof (long);
	else if (v->dtype == U_LONG)
		v->bsize = v->capacity * sizeof (t_ul);
}

static void set_bsize2(t_vector *v)
{
	if (dtype == LONG_LONG)
		v->bsize = v->capacity * sizeof (t_ll);
	else if (dtype == U_LONG_LONG)
		v->bsize = v->capacity * sizeof (t_ull);
	else if (dtype == U_LONG_LONG)
		v->bsize = v->capacity * sizeof (t_ull);
	else if (dtype == FLOAT)
		v->bsize = v->capacity * sizeof (float);
	else if (dtype == DOUBLE)
		v->bsize = v->capacity * sizeof (double);
	else if (dtype == LONG_DOUBLE)
		v->bsize = v->capacity * sizeof (t_ld);
	else if (dtype == VOID)
		v->bsize = v->capacity * sizeof (void);
}
