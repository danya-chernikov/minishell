#include "test.h"

/* To test get_par_by_prompt_ind() and get_token_by_prompt_ind() */
void	getters_tester(t_parser_data *d)
{
	size_t		i;
	size_t		prompt_ind;
	int			ptc; // parenthesis type as char
	int			choice;
	t_par_type	ptype;

	i = 0;
	while (i < 100)
	{
		printf("1 - get_par_by_prompt_ind() | 2 - get_token_by_prompt_ind()\n");
		printf("Enter function to test (1 | 2): ");
		scanf("%d", &choice);

		if (choice == 1)
		{
			printf("Enter prompt index: ");
			scanf("%lu", &prompt_ind);

			printf("Enter parenthesis type ( 1 - '(' | 2 - ')' ): ");
			//ptc = (char)getchar();
			scanf("%d", &ptc);
			if (ptc == 1)
				ptype = OPENING_PAR;
			else if (ptc == 2)
				ptype = CLOSING_PAR;

			printf("The index in parentheses array: %lld\n",
				get_par_by_prompt_ind(d, prompt_ind, ptype));
		}
		else if (choice == 2)
		{	
			printf("Enter prompt index: ");
			scanf("%lu", &prompt_ind);
			printf("The index in tokens array: %lld\n",
				get_token_by_prompt_ind(d, prompt_ind));
		}
		else
			continue;
		++i;
	}
}
