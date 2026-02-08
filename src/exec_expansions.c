#include "exec.h"
#include "shell.h"
#include "operand.h"

#include "vector.h"
#include "libft.h"

void	exec_expand_tilde(t_shell *msh, t_vector *vec_pair[], t_ind_type state)
{
	size_t		j;
	char		*tilde;
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[0];
	qmask = vec_pair[1];
	tilde = env_get_val(&msh->env, "~");
	j = 0;
	// Expand ~ by copying its value into the new array
	while (j < ft_strlen(tilde))
	{
		vector_push_back_char(exp_res, tilde[j]); // Copying...
		// Assign the corresponding index in qmask[] the curret `state` value
		vector_push_back_char(qmask, (char)state);
		++j;
	}
}

void	exec_expand_variable(t_shell *msh, t_vector *vec_pair[], char *var_name, t_ind_type state)
{
	size_t		j;
	char		*var;
	t_vector	*exp_res;
	t_vector	*qmask;

	exp_res = vec_pair[0];
	qmask = vec_pair[1];
	var = env_get_val(&msh->env, var_name);
	j = 0;
	// Expand $ by copying the value of `var` into the new array
	while (j < ft_strlen(var))
	{
		vector_push_back_char(exp_res, var[j]); // Copying...
		// Assign the corresponding index in qmask[] the curret `state` value
		vector_push_back_char(qmask, (char)state);
		++j;
	}
}
