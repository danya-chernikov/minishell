#ifndef TOKEN_H
# define TOKEN_H

# include <stddef.h>

# include "aux_common.h"

# define TOKEN_PIPE			"|"
# define TOKEN_OPEN_PAR		"("
# define TOKEN_CLOSE_PAR	")"
# define TOKEN_AND			"&&"
# define TOKEN_OR			"||"

# define MAX_TOKEN_LEN		512 // Maximum length of each token
# define MAX_TOKENS_NUM		256 // Maximum number of tokens

typedef struct s_parser_data	t_parser_data;
typedef struct s_operand		t_operand;

typedef enum e_token_type
{
    OPERAND,
    PIPE,
    OPEN_PAR,
    CLOSE_PAR,
	AND,
	OR,
	NONE // No tokens were found yet
	
}   t_token_type;

/* If this token's type is OPERAND we store
 * the pointer to the corresponding operand.
 *
 *     start_pi - Index of the first character
 *				  in the prompt string with
 *				  which this operand starts */
typedef struct s_token
{
	t_token_type	type;
	t_operand		*op;
	size_t			start_pi;

}	t_token;

void	tokens_init(t_parser_data *d);
t_ll	get_token_by_prompt_ind(t_parser_data *d, size_t prompt_ind);

#endif
