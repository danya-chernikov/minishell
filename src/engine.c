#include "engine.h"
#include "heredoc.h"
#include "exec.h"
#include "quote.h"
#include "debug.h"

#include <linux/limits.h> // For PATH_MAX

/* If the function returns -1, it means a critical error occurred,
 * and the caller should react by calling exit(EXIT_FAILURE);
 *
 * If it returns 0, a non-critical parser error occurred, and we
 * only need to prompt the user to enter another command in the shell;
 *
 * If it returns 1, everything is fine.
 * The parser_engine() is recursive function! */
int	shell_engine(t_shell *msh, char *prompt, int *ret_code)
{
	t_parser_data	*pdata;
	int				fres; // Returned code from function
	int				try;
	
	(void)ret_code;
	try = 0;
	fres = COMMON_SUCCESS;
	ft_bzero(&pdata, sizeof(pdata)); // We don't need it anymore..
	pdata = msh->pd;
	while (try < 1)
	{
		if (!prompt || ft_strlen(prompt) == 0)
			break ;

		fres = parser_init(pdata, prompt);
		if (fres != COMMON_SUCCESS) // Non-critial parser error
			break ;

		// Let's parse all quotes intervals for entered prompt
		fres = quotes_parser(pdata->prompt, pdata->quotes, &pdata->qpair_cnt);
		if (fres != COMMON_SUCCESS)
			break ;

		// May be launched only after quote intervals will be parsed
		fres = comments_parser(pdata);

		fres = check_empty_par(pdata);
		if (fres != COMMON_SUCCESS)
			break ;

		fres = parser_engine(pdata);
		if (fres != COMMON_SUCCESS) // If we got non-critical parser error
			break ; // Just prompt user to enter another command(s)

#if DEBUG == 1
		dbg_prompt_parser_print_all(pdata);
#endif

		// Let's parser all quotes intervals for each
		// operand-program, i.e. we're kinda updating them
		fres = operands_quotes_parser(pdata);
		if (fres != COMMON_SUCCESS)
			break ;

		fres = redirections_parser(pdata);
		if (fres != COMMON_SUCCESS)
			break ;

		fres = read_heredocs(msh);
		if (fres != COMMON_SUCCESS)
			break ;

#if DEBUG == 1
		dbg_print_redirs(pdata);
#endif

		/*fres = exec_ops(&pdata, ret_code);
		if (fres != COMMON_SUCCESS)
			break ;

		// Close all pipes of this prompt
		fres = close_pipes(&pdata);*/

		++try;
	} // End try block

	parser_free(pdata);
	return (fres);
}

/* Finds the first comment symbol #
 * that is located outside any quote
 * interval and truncates `prompt`
 * buffer of `s_parser_data` structure
 * eliminating all what goes after this
 * comment symbol.  May be launched only
 * after quote intervals will be parsed
 * calling quotes_parser() */
int	comments_parser(t_parser_data *d)
{
	size_t	pi;

	pi = 0;
	while (pi < ft_strlen(d->prompt))
	{
		if (d->prompt[pi] == '#' && !is_inside_quotes(d, pi))
		{
			d->prompt[pi] = '\0';
			break ;
		}
		++pi;
	}
	return (COMMON_SUCCESS);
}

/* Upates quote indexes for
 * each operand */
int	operands_quotes_parser(t_parser_data *d)
{
	t_operand	*op;
	size_t		ti;

	ti = 0;
	while (ti < d->token_cnt)
	{
		if (d->tokens[ti].type == OPERAND)
		{
			op = d->tokens[ti].op;
			remove_left_spaces(op->name);
			remove_right_spaces(op->name);
			if (!quotes_parser(op->name, op->quotes, &op->qpair_cnt))
				return (COMMON_FAILURE);
		}
		++ti;
	}
	return (COMMON_SUCCESS);
}
