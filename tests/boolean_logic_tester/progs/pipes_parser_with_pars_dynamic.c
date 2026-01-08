/* This program implements parsing of a basic user prompt
 * containing pipes with parentheses. For simplicity, it
 * launches programs located in the current directory. Each
 * program is named with a single letter of the English
 * alphabet. Lowercase-letter programs always return 0 (success),
 * while uppercase-letter programs always return 1 (failure) */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <stdbool.h>
# include <stdint.h>

# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

# include <readline/readline.h>
# include <readline/history.h>

typedef long long       t_ll;

# define EXIT_CMD                       "exit"
# define SUBSHELLS_PIDS_LOG     "subshells_PIDs"
# define BUF_SIZE                       1024

# define MAX_FORMAT_STR_LEN     64
# define PROMPT_INV_LEN         64      // Maximum length of user's prompt invitation string
# define MAX_PIPES_NUM          128
# define MAX_OPS_NUM            128
# define MAX_PAR_NUM            128     // Maximum parentheses number
# define MAX_TOKENS_NUM         128 // Maximum number of tokens
# define MAX_SUBSHS_NUM         128 // Maximum number of subshells
# define MAX_TOKEN_LEN          128 // Maximum length of each token

# define READ_END                       0
# define WRITE_END                      1
# define NOT_CLOSED_PAR         0       // This parenthesis wasn't closed yet (We didn't pass it)
# define CLOSED_PAR                     1       // Thie parenthesis was already closed

# define DEFAULT_FD                     -1
# define NONE_INDEX                     -1
# define NONE_PIPE                      -1
# define NONE_PAR_IND           -1

# define TOKEN_PIPE                     "|"
# define TOKEN_OPEN_PAR         "("
# define TOKEN_CLOSE_PAR        ")"
# define TOKEN_AND                      "&&"
# define TOKEN_OR                       "||"

/* STDIN_FILENO always must be bonded with read-end;
 * STDOUT_FILENO always must be bonded with write-end */
typedef struct s_operand
{
        char    name[2];        // Path to program
        int             read_end;       // stdin
        int             write_end;      // stdout
        pid_t   pid;
}       t_operand;

/* The value of -1 means the
 * index was not assigned */
typedef struct s_pair
{
        int     first;
        int     second;
}       t_pair;

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

/* Parenthesis type */
typedef enum e_par_type
{
        OPENING_PAR,
        CLOSING_PAR
}       t_par_type;

/* If this token's type is OPERAND we store
 * the pointer to the corresponding operand.
 *
 *     start_pi - Index of the first character
 *                                in the prompt string with
 *                                which this operand starts
 * */
typedef struct s_token
{
        t_token_type    type;
        t_operand               *op;
        size_t                  start_pi;
}       t_token;

typedef struct s_engine_data
{
        char            *prompt;                                        // Prompt entered by user
        size_t          pi;                                                     // Prompt index

        // PIPES
        size_t          pipe_cnt;                                       // Pipe counter
        size_t          max_pipes;                                      // Nuevo: para saber cu nto malloc hicimos
        int                     (*pipes)[2];                            // Puntero a arrays de 2 ints (para mantener la sintaxis pipes[i][0])
        //int                   pipes[MAX_PIPES_NUM][2];        // All pipes array

        // OPERANDS
        size_t          op_cnt;                                         // Operand counter
        size_t          max_ops;                                        // Nuevo
        t_operand       *ops;                                           // Puntero en lugar de array fijo
        //t_operand     ops[MAX_OPS_NUM];                       // Operands (programs to launch)

        // PARENTHESIS (Open)
        size_t          opar_num;                                       // Number of all opening-parentheses
        size_t          max_opar;                                       // Nuevo
        size_t          (*all_open_pars)[2];            // Puntero a arrays de 2 size_t
        //size_t                all_open_pars[MAX_PAR_NUM][2]; // Indexes of all opening-parentheses

        // PARENTHESIS (Stack)
        int                     opar_cnt;                                       // Opening-parentheses counter (must be int)
        size_t          *open_par;
        // size_t               open_par[MAX_PAR_NUM];          // Opening-parentheses indexes found and thier flags

        // PARENTHESIS (Close)
        int                     cpar_cnt;                                       // Closing-parentheses counter (for now let it be int)
        size_t          max_cpar;                                       // Nuevo
        size_t          (*close_par)[2];                        // Puntero
        //size_t                close_par[MAX_PAR_NUM][2];      // Closing-parentheses indexes found and their flags

        // PAIRS
        //t_pair                pars[MAX_PAR_NUM];                      // A member that represents each parentheses pair
        size_t          par_cnt;                                        // Parentheses pair counter
        t_pair          *pars;                                          // Punetero
        size_t          max_pairs;                                      // Nuevo

        // TOKENS
        size_t          token_cnt;                                      // Token counter
        size_t          max_tokens;                                     // Nuevo
        t_token         *tokens;                                        // Puntero
        //t_token               tokens[MAX_TOKENS_NUM];         // Here we store all tokens we found during parsing

}       t_engine_data;

/* Initialization */
int                     parser_init(t_engine_data *d, char *rline_buf);
//void                  init_ops(t_operand *ops);
void                    init_ops(t_engine_data *d);
void                    init_open_par(t_engine_data *d);
void                    init_close_par(t_engine_data *d);
void                    init_tokens(t_engine_data *d);
//void                  init_pars(t_pair *pars);
void                    init_pars(t_engine_data *d);
void                    remove_right_spaces(char *prompt);
bool                    check_empty_par(char *prompt);

/* Parser engine */
bool                    parser_engine(t_engine_data *d);
void                    handle_open_par(t_engine_data *d, int opar_ind, bool *f_noerr);
void                    handle_close_par(t_engine_data *d, bool *f_noerr);
int                     later_goes_open_par(char *str, size_t ind);
void                    skip_spaces(char *prompt, size_t *pi);

/* Execution flow */
int                     exec_ops(t_engine_data *d); // Jhon
t_ll                    get_par_by_prompt_ind(t_engine_data *d, size_t prompt_ind, t_par_type ptype);
t_ll                    get_token_by_prompt_ind(t_engine_data *d, size_t prompt_ind);
int                     close_pipes(t_engine_data *d); // Jhon

/* Testing */
void                    getters_tester(t_engine_data *d);

/* Debugging */
void                    print_parsed_data(t_engine_data *d);
void                    print_tokens(t_engine_data *d);
void                    print_parentheses(t_engine_data *d);
int                     parser_init_dynamic(t_engine_data *d, char *rline_buf); // John
void                    free_parser_data(t_engine_data *d); // John

/*
int     main(void)
{
        t_engine_data   eng_data;
        char                    prompt[PROMPT_INV_LEN];
        char                    *rline_buf;

        rline_buf = NULL;
        strncpy(prompt, "dchernik@c3r3s6: ", PROMPT_INV_LEN);
        while (1) // readline loop
        {
                rline_buf = readline(prompt);
                //printf("rline_buf = %p\n", rline_buf);
                //printf("rline_buf = \"%s\"\n", rline_buf);
                if (strlen(rline_buf) == 0)
                {
                        //printf("rline_buf is NULL\n");
                        free(rline_buf);
                        rline_buf = NULL;
                        continue;
                }
                add_history(rline_buf);
                if (!strncmp(rline_buf, EXIT_CMD, strlen(EXIT_CMD)) &&
                        strlen(rline_buf) == strlen(EXIT_CMD))
                {
                        free(rline_buf);
                        rline_buf = NULL;
                        break;
                }

                // Let's analyze the received prompt/request

                if (!parser_init(&eng_data, rline_buf))
                        continue;

                if (!parser_engine(&eng_data)) // If we got non-critical parser error
                        continue; // Just prompt user to enter another command(s)

                print_parsed_data(&eng_data);
                print_tokens(&eng_data);
                print_parentheses(&eng_data);

                //getters_tester(&eng_data);

                if (!exec_ops(&eng_data))
                        exit(EXIT_FAILURE);

        // Close all pipes of this prompt
                if (!close_pipes(&eng_data))
                        exit(EXIT_FAILURE);

                free(rline_buf);
                rline_buf = NULL;

        } // while (1) // readline loop
        return 0;
}
*/

int     main(void)
{
        t_engine_data           eng_data;
        char                    prompt[PROMPT_INV_LEN];
        char                    *rline_buf;

        rline_buf = NULL;
        strncpy(prompt, "dchernik@c3r3s6: ", PROMPT_INV_LEN);
        while (1) // readline loop
        {
                rline_buf = readline(prompt);
                if (rline_buf == NULL)
                {
                        printf("exit\n");
                        break ;
                }
                if (strlen(rline_buf) == 0)
                {
                        free(rline_buf);
                        rline_buf = NULL;
                        continue;
                }
                add_history(rline_buf);
                if (!strncmp(rline_buf, EXIT_CMD, strlen(EXIT_CMD)) &&
                        strlen(rline_buf) == strlen(EXIT_CMD))
                {
                        free(rline_buf);
                        rline_buf = NULL;
                        break;
                }

                // Let's analyze the received prompt/request

                /*if (!parser_init(&eng_data, rline_buf))
                        continue;*/
                if (parser_init_dynamic(&eng_data, rline_buf) == 0)
                {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(rline_buf);
                        continue ;
                }

                if (parser_engine(&eng_data) == 0)
                {
                        free_parser_data(&eng_data);
                        free(rline_buf);
                        continue ;
                }
                print_parsed_data(&eng_data);
                print_tokens(&eng_data);
                print_parentheses(&eng_data);

                //getters_tester(&eng_data);

                if (!exec_ops(&eng_data))
                        exit(EXIT_FAILURE);

        // Close all pipes of this prompt
                if (!close_pipes(&eng_data))
                        exit(EXIT_FAILURE);

                free_parser_data(&eng_data);
                free(rline_buf);
                rline_buf = NULL;

        } // while (1) // readline loop
        return 0;
}

/* The first token is always NONE because
 * we always check previous token value */
int     parser_init(t_engine_data *d, char *rline_buf)
{
        d->pi                   = 0;
        d->op_cnt               = 0;
        d->pipe_cnt             = 0;
        d->opar_num             = 0;
        d->opar_cnt             = 0;
        d->cpar_cnt             = 0;
        d->par_cnt              = 0;
        d->token_cnt    = 1;
        d->prompt               = rline_buf;

        remove_right_spaces(d->prompt);

        init_ops(d); // Initialize operators array
        init_open_par(d);
        init_close_par(d);
        init_tokens(d);
        init_pars(d);

        if (!check_empty_par(d->prompt))
        {
                fprintf(stderr, "Parsing error: "
                        "Empty parentheses are not permitted\n");
                return 0;
        }
        return 1;
}

/* Assign the default value to
 * the pipes of all operators */
/*
void    init_ops(t_operand *ops)
{
        size_t  i;

        i = 0;
        while (i < MAX_OPS_NUM)
        {
                ops[i].write_end = DEFAULT_FD;
                ops[i].read_end = DEFAULT_FD;
                ++i;
        }
}
*/
void    init_ops(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < d->max_tokens)
        {
                d->ops[i].write_end = DEFAULT_FD;
                d->ops[i].read_end = DEFAULT_FD;
                ++i;
        }
}

/*
void    init_open_par(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < strlen(d->prompt))
        {
                if (d->prompt[i] == '(')
                {
                        d->all_open_pars[d->opar_num][0] = i;
                        d->all_open_pars[d->opar_num][1] = NOT_CLOSED_PAR;
                        ++d->opar_num;
                }
                ++i;
        }
        i = 0;
        while (i < d->opar_num)
        {
                d->open_par[i] = NONE_INDEX;
                ++i;
        }
}
*/
void    init_open_par(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < strlen(d->prompt))
        {
                if (d->prompt[i] == '(')
                {
                        if (d->opar_num >= d->max_tokens)
                        {
                                fprintf(stderr, "Error: Too many parentheses for internal buffer\n");
                                break ;
                        }
                        d->all_open_pars[d->opar_num][0] = i;
                        d->all_open_pars[d->opar_num][1] = NOT_CLOSED_PAR;
                        ++d->opar_num;
                }
                ++i;
        }
        i = 0;
        while (i < d->opar_num)
        {
                d->open_par[i] = NONE_INDEX;
                ++i;
        }
}

/* Counts all closing parentheses and remembers their indexes */
/*
void    init_close_par(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < strlen(d->prompt))
        {
                if (d->prompt[i] == ')')
                {
                        d->close_par[d->cpar_cnt][0] = i;
                        d->close_par[d->cpar_cnt][1] = NOT_CLOSED_PAR;
                        ++d->cpar_cnt;
                }
                ++i;
        }
}
*/
void    init_close_par(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < strlen(d->prompt))
        {
                if (d->prompt[i] == ')')
                {
                        if (d->opar_num >= d->max_tokens)
                        {
                                fprintf(stderr, "Error: Too many parentheses for internal buffer\n");
                                break ;
                        }
                        d->close_par[d->cpar_cnt][0] = i;
                        d->close_par[d->cpar_cnt][1] = NOT_CLOSED_PAR;
                        ++d->cpar_cnt;
                }
                ++i;
        }
}

/*
void    init_tokens(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < MAX_TOKENS_NUM)
        {
                d->tokens[i].op = NULL;
                ++i;
        }
        d->tokens[0].type = NONE;
        d->tokens[0].start_pi = 0;
}
*/
void    init_tokens(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < d->max_tokens)
        {
                d->tokens[i].op = NULL;
                ++i;
        }
        if (d->max_tokens > 0)
        {
                d->tokens[0].type = NONE;
                d->tokens[0].start_pi = 0;
        }
}

/* Let's say the first element of the
 * pair is the opening parenthesis,
 * and the second is the closing one */
/*
void    init_pars(t_pair *pars)
{
        size_t  i;

        i = 0;
        while (i < MAX_PAR_NUM)
        {
                pars->first = NONE_PAR_IND;
                pars->second = NONE_PAR_IND;
                ++i;
        }
}
*/
void    init_pars(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < d->max_tokens)
        {
                d->pars[i].first = NONE_PAR_IND;
                d->pars[i].second = NONE_PAR_IND;
                ++i;
        }
}

void    remove_right_spaces(char *prompt)
{
        int     i;

        i = strlen(prompt) - 1;
        if (prompt[i] == ' ')
        {
                while (i >= 0 && prompt[i] == ' ')
                {
                        prompt[i] = '\0';
                        --i;
                }
        }
}

/* Checks for existance of empty parentheses.
 * Sequences like: (), (( )), (((  ))), and etc.
 * Returns true if there are no empty sequences*/
bool    check_empty_par(char *prompt)
{
        size_t  i;

        i = 0;
        while (i < strlen(prompt))
        {
                if (prompt[i] == '(' )
                {
                        ++i;
                        skip_spaces(prompt, &i);
                        if (i == strlen(prompt)) // Parsing error
                                return false;
                        if (prompt[i] == ')')
                                return false;
                }
                ++i;
        }
        return true;
}

/* Parses the user's prompt string by connecting all
 * operands with pipes and launching or exiting subshells
 * when encountering '(' or ')' parentheses, respectively */
bool parser_engine(t_engine_data *d)
{
        size_t  prompt_len;
        bool    f_noerr;        // Parsing error flag
        int     opar_ind;       // Prompt index of the open-parenthesis that goes after pipe

        f_noerr = true; // Let's assume there are no errors at first
        prompt_len = strlen(d->prompt);
        while (d->pi < prompt_len) // Going through the entered prompt string
        {
                if (d->prompt[d->pi] == ' ')
                        skip_spaces(d->prompt, &d->pi); // Skip possible spaces

                // If reached the end of the prompt
                if (d->pi == prompt_len)
                        break ;

                // If it's letter
                if (isalpha(d->prompt[d->pi]))
                {
                        // Letter-operand can go only after
                        // pipe or be the first token or
                        // go after '('
                        /*if (d->tokens[d->token_cnt - 1].type != NONE &&
                                d->tokens[d->token_cnt - 1].type != PIPE &&
                                d->tokens[d->token_cnt - 1].type != OPEN_PAR)*/
                        if (d->tokens[d->token_cnt - 1].type != NONE &&
                                d->tokens[d->token_cnt - 1].type != PIPE &&
                                d->tokens[d->token_cnt - 1].type != OPEN_PAR &&
                                d->tokens[d->token_cnt - 1].type != AND &&
                                d->tokens[d->token_cnt - 1].type != OR)
                        {
                                // Situations like:
                                // "a | (b | c) d"
                                // "a | b c"
                                // "a (b | c)"
                                f_noerr = false;
                                fprintf(stderr, "Parsing error: "
                                        "Before operand must go pipe, &&, || or '('\n");
                                break ;
                        }

                        // Add this letter in the operators array
                        d->ops[d->op_cnt].name[0] = d->prompt[d->pi];
                        d->ops[d->op_cnt].name[1] = '\0';

                        // Add this operand into the tokens array
                        d->tokens[d->token_cnt].type = OPERAND;
                        d->tokens[d->token_cnt].op = (t_operand *)&d->ops[d->op_cnt];
                        d->tokens[d->token_cnt].start_pi = d->pi;
                        ++d->token_cnt;

                        ++d->op_cnt;

                        ++d->pi; // Move one symbol forward in prompt

                        // Let's see what goes next
                        skip_spaces(d->prompt, &d->pi);

                        // It means nothing is on the right (just spaces)
                        if (d->pi == prompt_len) // We reached the end of the prompt
                        {
                                if (d->pipe_cnt > 0) // If it's the last operand in the prompt
                                {
                                        d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;
                                        d->ops[d->op_cnt - 1].write_end = NONE_PIPE;
                                }
                                // Otherwise, it means our prompt contains only one letter-operand
                                break ;
                        }
                        // Let's see what goes after the letter
                        // After the letter goes a pipe
                        if (d->prompt[d->pi] == '|' && d->prompt[d->pi + 1] != '|') // If further goes pipe
                        {
                                // A pipe can go only after an operand or after a ')'
                                if (d->tokens[d->token_cnt - 1].type != OPERAND &&
                                        d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
                                {
                                        // Situations like:
                                        // "a ( | b"
                                        // "a | | b"
                                        f_noerr = false;
                                        fprintf(stderr, "Parsing error: "
                                                "A pipe can go only after ')' or an operand\n");
                                        break ;
                                }

                                // Add this operand into the tokens array
                                d->tokens[d->token_cnt].type = PIPE;
                                d->tokens[d->token_cnt].start_pi = d->pi;
                                ++d->token_cnt;

                                // Let's create a pipe
                                if (pipe(&d->pipes[d->pipe_cnt][0]) == -1)
                                {
                                        fprintf(stderr, "Can't create pipe: %s\n", strerror(errno));
                                        exit(EXIT_FAILURE);
                                }

                                if (d->pipe_cnt == 0) // If it's the first operand found
                                        d->ops[d->op_cnt - 1].read_end = NONE_PIPE;
                                else // It's not the first operand
                                // Assign to its stdin the previous pipe index
                                        d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;

                                // Assign to its stdout the current pipe index
                                d->ops[d->op_cnt - 1].write_end = d->pipe_cnt;

                                ++d->pipe_cnt;  // Increment pipe index

                                opar_ind = later_goes_open_par(d->prompt, d->pi);
                                // If after pipe goes opening-parenthesis '('
                                if (opar_ind != -1)
                                {
                                        handle_open_par(d, opar_ind, &f_noerr);
                                        if (!f_noerr)
                                                break ;
                                        else
                                        {
                                                // If we are here it means ')' was found
                                                if (d->tokens[d->token_cnt - 1].type == CLOSE_PAR &&
                                                        d->pi == prompt_len)
                                                {
                                                        if (d->pipe_cnt > 0)
                                                                d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;
                                                }
                                                continue ; // Go further by prompt
                                        }
                                }

                        } // d->prompt[pi] == '|' // After the letter goes pipe

                        else // if (prompt[pi] != '|') // After letter goes not pipe
                        {
                                // If after the letter goes closing-parenthesis ')'
                                if (d->prompt[d->pi] == ')')
                                {
                                        handle_close_par(d, &f_noerr);
                                        break ;
                                }
                                else if (d->pi + 1 < prompt_len && d->prompt[d->pi] == '&' && d->prompt[d->pi + 1] == '&')
                                {
                                        d->tokens[d->token_cnt].type = AND;
                                        d->tokens[d->token_cnt].start_pi = d->pi;
                                        d->token_cnt++;
                                        d->pi = d->pi + 2; // Saltamos los dos caracteres &&
                                        continue ;
                                }
                                else if (d->pi + 1 < prompt_len && d->prompt[d->pi] == '|' && d->prompt[d->pi + 1] == '|')
                                {
                                        d->tokens[d->token_cnt].type = OR;
                                        d->tokens[d->token_cnt].start_pi = d->pi;
                                        d->token_cnt++;
                                        d->pi = d->pi + 2; // saltamos los dos caracteres ||
                                        continue ;
                                }
                                else // If after the letter goes neither '|' nor ')'
                                {
                                        f_noerr = false;
                                        if (isalpha(d->prompt[d->pi]))
                                        {
                                                fprintf(stderr, "Parsing error: "
                                                        "After operand cannot go another operand\n");
                                        }
                                        else
                                        {
                                                fprintf(stderr, "Parsing error. What is '%c' ?\n",
                                                        d->prompt[d->pi]);
                                        }
                                        break ;
                                }
                        }

                } // if ((prompt[pi] >= 'a' && prompt[pi] <= 'z')

                else // If it's not a letter
                {
                        // In case the first symbol going after omitted spaces is '('
                        // Or in other words
                        // If the user command(s) starts with an opening parenthesis
                        if (d->prompt[d->pi] == '(') // For example: (a | b) | c
                        {
                                // A '(' can go only after a pipe or another '('
                                // or also be the first token found
                                /*if (d->tokens[d->token_cnt - 1].type != NONE &&
                                        d->tokens[d->token_cnt - 1].type != PIPE &&
                                        d->tokens[d->token_cnt - 1].type != OPEN_PAR) // ~(A + B) = ~A * ~B*/
                                if (d->tokens[d->token_cnt - 1].type != NONE &&
                                        d->tokens[d->token_cnt - 1].type != PIPE &&
                                        d->tokens[d->token_cnt - 1].type != OPEN_PAR &&
                                        d->tokens[d->token_cnt - 1].type != AND &&
                                        d->tokens[d->token_cnt - 1].type != OR)
                                {
                                        // Situations like:
                                        // "a | (b | c)(d | e)"
                                        // "a | b | c ( | a)"
                                        f_noerr = false;
                                        fprintf(stderr, "Parsing error: "
                                                "A '(' can go only after a pipe, &&, || or another '('\n");
                                        break ;
                                }

                                handle_open_par(d, d->pi, &f_noerr);
                                if (!f_noerr)
                                        break ;
                                else
                                {
                                        // If we are here it means ')' was found
                                        if (d->tokens[d->token_cnt - 1].type == CLOSE_PAR &&
                                                d->pi == prompt_len)
                                        {
                                                if (d->pipe_cnt > 0)
                                                        d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;
                                        }
                                        continue ; // Go further by prompt
                                }
                        }
                        else if (d->prompt[d->pi] == ')') // If it's closing-parenthesis
                        {
                                handle_close_par(d, &f_noerr);
                                break ;
                        }
                        else if (d->prompt[d->pi] == '|' && d->prompt[d->pi     + 1] != '|') // If it's pipe
                        {
                                // Pipe can go only after a ')' or after an operand
                                if (d->tokens[d->token_cnt - 1].type != CLOSE_PAR &&
                                        d->tokens[d->token_cnt - 1].type != OPERAND)
                                {
                                        // Situations like:
                                        // "a | (b | c)( | d"
                                        // "a | b | | c"
                                        f_noerr = false;
                                        fprintf(stderr, "Parsing error: "
                                                "Pipe can go only after a ')' or an operand\n");
                                        break;
                                }

                                // Add this operand into the tokens array
                                d->tokens[d->token_cnt].type = PIPE;
                                d->tokens[d->token_cnt].start_pi = d->pi;
                                ++d->token_cnt;

                                // Let's create a pipe
                                if (pipe(&d->pipes[d->pipe_cnt][0]) == -1)
                                {
                                        fprintf(stderr, "Can't create pipe: %s\n", strerror(errno));
                                        exit(EXIT_FAILURE);
                                }

                                // Assign to its stdin the previous pipe index
                                d->ops[d->op_cnt - 1].read_end = d->pipe_cnt - 1;

                                // Assign to its stdout the current pipe index
                                d->ops[d->op_cnt - 1].write_end = d->pipe_cnt;

                                ++d->pipe_cnt; // Increment pipe index

                                // Go further by prompt

                        } // else if (d->prompt[d->pi] == '|') // If it's pipe
                        else if (d->pi + 1 < prompt_len && d->prompt[d->pi] == '&' && d->prompt[d->pi + 1] == '&')
                        {
                                if (d->tokens[d->token_cnt - 1].type != OPERAND && d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
                                {
                                        fprintf(stderr, "Parsing error: Unexpected &&\n");
                                        f_noerr = false;
                                        break ;
                                }
                                d->tokens[d->token_cnt].type = PIPE;
                                d->tokens[d->token_cnt].start_pi = d->pi;
                                d->token_cnt++;
                                d->pi = d->pi + 2;
                                continue ;
                        }
                        else
                        {
                                f_noerr = false;
                                fprintf(stderr, "Parsing error. What is '%c' ?\n", d->prompt[d->pi]);
                                break;
                        }
                } // else // If it's not a letter

                ++d->pi;
        } // while (pi < prompt_len)

        return f_noerr;
} // parser_engine() function

/* Handles opening-parenthesis */
void    handle_open_par(t_engine_data *d, int opar_ind, bool *f_noerr)
{
        size_t  last_opar_ind;
        size_t  prompt_len;
        size_t  i;

        // Add this operand into the tokens array
        d->tokens[d->token_cnt].type = OPEN_PAR;
        d->tokens[d->token_cnt].start_pi = opar_ind;
        ++d->token_cnt;

        prompt_len = strlen(d->prompt);
        // Add its prompt index to the opening-parentheses array
        d->open_par[d->opar_cnt] = opar_ind;

        // Add this opening-parenthesis index to
        // the pair of all all parentheses pairs
        d->pars[d->par_cnt].first = opar_ind;
        ++d->par_cnt;

        // Move to the next symbol in the prompt after '('
        d->pi = opar_ind + 1;

        ++d->opar_cnt; // Increment opening-parenthesis counter

        // When prompt like this "a | b | (" for example
        if (d->pi == prompt_len)
        {
                *f_noerr = false;
                fprintf(stderr, "Parsing error: "
                        "prompt terminates with '('\n"); // Reword this
                return; // Go further by prompt
        }

        if (!parser_engine(d))
        {
                *f_noerr = false;
                return;
        }
        /* If we're here the child process was
         * terminated (most likely when it
         * encountered a closing-parenthesis).
         * Now we have to omit all symbols between
         * this ')' and the last found '('.
         * Furthermore, we must remove the index
         * of the last found '(' from `d->opar`
         * and decrement `d->opar_cnt` */

        // If the closing-parentheses array is empty
        if (d->cpar_cnt == 0)
        {
                *f_noerr = false;
                fprintf(stderr, "Parsing error: "
                        "Some '(' were found but there are no any ')' to match them\n");
                return; // Go further by prompt
        }

        // Let's find, in the closing-parentheses array, the nearest
        // ')' that is not marked as closed to the last found '(' and
        // that is located on the right from '('
        last_opar_ind = d->open_par[d->opar_cnt - 1];
        i = 0;
        // The closing-parentheses array is already sorted
        //while (i < d->cpar_cnt)
        while (i < (size_t)d->cpar_cnt)
        {
                if (d->close_par[i][0] > last_opar_ind &&
                        d->close_par[i][1] == NOT_CLOSED_PAR)
                        break;
                ++i;
        }

        //if (i == d->cpar_cnt)
        if (i == (size_t)d->cpar_cnt) // We went out of the array border
        {
                *f_noerr = false;
                fprintf(stderr, "Parsing error: "
                        "Some '(' were found but there are no any ')' to match them\n");
                return; // Go further by prompt
        }

        // Move the prompt index to the next symbol in the
        // prompt after the nearest ')' to the last '(' found
        d->pi = d->close_par[i][0] + 1;

        // Mark this closing-parenthesis as closed
        d->close_par[i][1] = CLOSED_PAR;

        // By decrementing the opening-parentheses counter we remove
        // the last element from the array of all opening-parentheses
        --d->opar_cnt;
}

void    handle_close_par(t_engine_data *d, bool *f_noerr)
{
        //int           i;
        size_t  i;
        size_t  last_cpar_ind;  // Last closing-parenthesis index
        size_t  pair_opar_ind;

        // A ')' can go only after an operand or after another ')'
        if (d->tokens[d->token_cnt - 1].type != OPERAND &&
                d->tokens[d->token_cnt - 1].type != CLOSE_PAR)
        {
                // Situations like:
                // "a (b | )"
                // "a | ()"
                *f_noerr = false;
                fprintf(stderr, "Parsing error: "
                        "A ')' can go only after an operand or another ')'\n");
        }

        // If the array of opening-parenthesis is empty
        if (d->opar_cnt == 0)
        {
                *f_noerr = false;
                fprintf(stderr, "Parsing error: "
                        "Some ')' were found but there are no any '(' to match them\n");
        }
        else
        {
                // Add this operand into the tokens array
                d->tokens[d->token_cnt].type = CLOSE_PAR;
                d->tokens[d->token_cnt].start_pi = d->pi;
                ++d->token_cnt;

                // Let's find the nearest to us (to `d->pi`)
                // not-yet-closed opening parenthesis to the
                // left from `d->pi` in `d->open_par`
                last_cpar_ind = d->pi;
                i = 0;
                pair_opar_ind = i;
                // The `opar_num` after calculating it on the
                // initialization stage will never be changed
                // meanwhile `opar_cnt` will be decreased
                // each time we find a closing-parenthesis
                while (i < d->opar_num)
                {
                        if (d->all_open_pars[i][0] < last_cpar_ind &&
                                d->all_open_pars[i][1] == NOT_CLOSED_PAR)
                        {
                                pair_opar_ind = i;
                        }
                        ++i;
                }

                // Add this closing parenthesis index to the
                // list of all parenthesis pairs to match the
                // corresponding opening parenthesis index
                d->pars[pair_opar_ind].second = d->pi;

                // Mark the matched opening-parenthesis as closed
                d->all_open_pars[pair_opar_ind][1] = CLOSED_PAR;
        }
}

/* Checks whether there is an opening parenthesis
 * later to the right, starting from index ind,
 * ignoring only spaces. If it finds '(' while
 * moving to the right and skipping all spaces,
 * it returns its index. Otherwise, it returns -1.
 * If it encounters any symbol other than a space
 * before finding '(', it also returns -1 */
int     later_goes_open_par(char *str, size_t ind)
{
        size_t  slen;

        ++ind;
        slen = strlen(str);
        while (ind < slen)
        {
                if (str[ind] == '(')
                        return ind;
                if (str[ind] == ' ')
                        ++ind;
                else
                        return -1;
        }
        return (-1);
}

void    skip_spaces(char *prompt, size_t *pi)
{
        while (prompt[*pi] == ' ' && *pi < strlen(prompt))
                ++(*pi);
}

/* Now we have to launch all operand-programs.
 * On error returns 0. On success returns 1 */

int     wait_for_pid(pid_t pid)
{
        int status;

        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
                return (WEXITSTATUS(status));
        return (1);
}

int skip_logic_blocks(t_engine_data *d, int current_i, int last_status)
{
        t_token_type type = d->tokens[current_i].type;
        int depth;

    // Logic:
    // If (&& and failed) OR (|| and success) -> SKIP
        if ((type == AND && last_status != 0) || (type == OR && last_status == 0))
        {
                depth = 0;
                current_i++; // Skip the operator (&& or ||)

                // Skip until we find the next operator that allows execution
                // We must respect parentheses nesting!
                while (current_i < (int)d->token_cnt)
                {
                        if (d->tokens[current_i].type == OPEN_PAR)
                                depth++;
                        else if (d->tokens[current_i].type == CLOSE_PAR)
                                depth--;
            // If we are at ground level (depth 0)
                        if (depth == 0)
                        {
                                // If we skipped because of AND (failure), we stop at OR
                                // because OR might want to run (false && ... || true)
                                if (type == AND && d->tokens[current_i].type == OR)
                                        break ;
                                // If we skipped because of OR (success), we stop at AND
                                // because AND might want to run (true || ... && true)
                                if (type == OR && d->tokens[current_i].type == AND)
                                        break ;
                        }
                        current_i++;
                }
        }
        else
        {
                // Logic says: EXECUTE. Just skip the operator itself.
                current_i++;
        }
        return (current_i);
}

int exec_ops(t_engine_data *d)
{
        int             i;
        int             k;
        int             last_status;
        pid_t   last_pid;
        int             pipe_start;
        int             pipe_end; // Rango del pipeline actual
        int             fd_log = open(SUBSHELLS_PIDS_LOG, O_WRONLY | O_CREAT | O_APPEND, 0644); // Archivo de log (mantenido del original)
        int             depth;
        int             close_idx;

        i = 0;
        last_status = 0;
        while (i < (int)d->token_cnt)
        {
                last_pid = -1;
                // 1. Identificar el siguiente segmento del Pipeline
                // Un segmento se extiende hasta encontrar un &&, un || o el final.
                pipe_start = i;
                pipe_end = i;
                while (pipe_end < (int)d->token_cnt &&
                                d->tokens[pipe_end].type != AND &&
                                d->tokens[pipe_end].type != OR)
                {
                        pipe_end++;
                }

                // 2. Ejecutar todos los comandos en este segmento [pipe_start, pipe_end)
                k = pipe_start;
                while (k < pipe_end)
                {
                        if (d->tokens[k].type == OPERAND)
                        {
                                last_pid = fork();
                                if (last_pid == 0)
                                {
                                        // Proceso Hijo
                                        t_operand       *op = d->tokens[k].op;

                                        // Configuraci n de redirecci n de Pipes
                                        if (op->write_end != -1)
                                                dup2(d->pipes[op->write_end][WRITE_END], STDOUT_FILENO);
                                        if (op->read_end != -1)
                                                dup2(d->pipes[op->read_end][READ_END], STDIN_FILENO);

                                        // IMPORTANTE: Cerrar todos los pipes en el hijo antes del execve
                                        close_pipes(d);

                                        // Ejecuci n del comando
                                        char    *argv[] = {op->name, NULL};
                                        char    *envp[] = {NULL}; // Entorno m nimo para pruebas
                                        execve(op->name, argv, envp);

                                        // Si execve falla (por ejemplo, comando no encontrado)
                                        fprintf(stderr, "Opps, %s failed\n", op->name);
                                        exit(127); // C digo est ndar para 'command not found'
                                }
                        }
                        else if (d->tokens[k].type == OPEN_PAR)
                        {
                                // L gica de Subshell (Par ntesis)
                                // Buscar el  ndice del par ntesis de cierre correspondiente
                                depth = 1;
                                close_idx = k + 1;
                                while (close_idx < pipe_end && depth > 0)
                                {
                                        if (d->tokens[close_idx].type == OPEN_PAR) depth++;
                                        if (d->tokens[close_idx].type == CLOSE_PAR) depth--;
                                        if (depth > 0) close_idx++;
                                }
                                last_pid = fork();
                                if (last_pid == 0)
                                {
                                        // Hijo de la Subshell
                                        if (fd_log != -1)
                                                dprintf(fd_log, "Subshell PID: %d\n", getpid());
                                        // Aqu  se deber a llamar recursivamente a exec_ops con el rango limitado.
                                        // Por ahora, simulamos  xito con exit(0).
                                        exit(0);
                                }
                                // Saltar los tokens dentro de los par ntesis en el bucle principal
                                k = close_idx;
                        }
                        k++;
                }

                // 3.  Cerrar pipes en el Padre!
                // Esto es crucial para evitar bloqueos (deadlocks).
                // Cerramos los pipes de este segmento ya que son de un solo uso.
                close_pipes(d);

                // 4. Esperar a que el pipeline termine
                // Solo nos interesa el estado de salida del  LTIMO comando del pipeline
                if (last_pid != -1)
                {
                        last_status = wait_for_pid(last_pid);
                        // Limpiar procesos restantes para evitar procesos zombis
                        while (wait(NULL) > 0);
                }

                // 5. Control L gico:  Debemos continuar o saltar el siguiente bloque?
                // Movemos el iterador a la posici n del operador (&& o ||)
                i = pipe_end;
                if (i < (int)d->token_cnt)
                {
                        // skip_logic_blocks decidir  si saltar el siguiente segmento seg n last_status
                        i = skip_logic_blocks(d, i, last_status);
                }
        }
        if (fd_log != -1)
                close(fd_log);
        return (1);
}


/*
int     exec_ops(t_engine_data *d)
{
        t_operand       *op;                                    // Pointer to the current operand
        pid_t           progs[MAX_OPS_NUM];             // Program PIDs
        pid_t           subshs[MAX_SUBSHS_NUM]; // Our subshells' PIDs
        int                     ti;                                             // Token index (must be int)
        size_t          sh_i;                                   // Subshell index
        size_t          pi;                                             // Program index
        t_ll            cpar_pars_ind;                  // Index of closing parenthesis in `d->pars`
        t_ll            opar_token_ind;                 // Index of corresponding opening parenthesis in `d->tokens`

        int                     fd_log;
        char            buf[BUF_SIZE];                  // Buffer to write into the log file
        ssize_t         nr;                                             // Number of bytes written to the log file

        // Create a log file to write the PIDs of all subshells we launch
        fd_log = open(SUBSHELLS_PIDS_LOG,
                                  O_WRONLY | O_CREAT | O_APPEND,
                                  S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH); // 644
        if (fd_log == -1)
        {
                fprintf(stderr, "Can't create/open the log file: %s\n", strerror(errno));
                return (0);
        }
        // Write the parent shell's PID into the log file
        snprintf(buf, BUF_SIZE, "Hi! I'm a parent shell! My PID is: %jd\n",
                (intmax_t)getpid());
        nr = write(fd_log, buf, strlen(buf));
        //if (nr != strlen(buf))
        if (nr != (ssize_t)strlen(buf))
        {
                fprintf(stderr, "Couldn't write into the log file\n");
                return (0);
        }

        // Traversing from right to left the tokens array
        pi = 0;
        sh_i = 0;
        ti = (int)(d->token_cnt) - 1;
        while (ti >= 0)
        {
                if (d->tokens[ti].type == OPERAND)
                {
                        progs[pi] = fork();
                        if (progs[pi] == -1)
                        {
                                fprintf(stderr, "Can't fork: %s\n", strerror(errno));
                                return (0);
                        }
                        else if (progs[pi] == 0) // 0 always is returned in the child
                        {
                                ++pi;

                                // We're in the new process
                                char    *op_argv[2] = { 0, 0 };
                                char    *envp[] = { "HOME=/home/user",
                                                                        "PATH=/bin:/usr/bin",
                                                                        "USER=user", 0 };

                                op = d->tokens[ti].op;
                                op_argv[0] = &op->name[0];

                                // Let's attach pipes to each process (operand)
                                if (op->write_end != -1)
                                        dup2(d->pipes[op->write_end][WRITE_END], STDOUT_FILENO);

                                if (op->read_end != -1)
                                        dup2(d->pipes[op->read_end][READ_END], STDIN_FILENO);

                                // Let's close all inherited parent's pipes

                                if (!close_pipes(d))
                                        return (0);

                                // Replace the executable image of this process
                                execve(op_argv[0], &op_argv[0], envp);

                                fprintf(stderr, "Opps, %s failed\n", op_argv[0]);
                                return (0);

                        } // else if (progs[op_i] == 0)
                }
                else if (d->tokens[ti].type == PIPE)
                {
                        // Do nothing
                }
                else if (d->tokens[ti].type == CLOSE_PAR)
                {
                        subshs[sh_i] = fork();
                        if (subshs[sh_i] == -1)
                        {
                                fprintf(stderr, "Can't fork: %s\n", strerror(errno));
                                return (0);
                        }
                        if (subshs[sh_i] == 0)
                        {
                                snprintf(buf, BUF_SIZE, "Hi! I'm a subshell! My PID is: %jd\n",
                                        (intmax_t)getpid());
                                nr = write(fd_log, buf, strlen(buf));
                                //if (nr != strlen(buf))
                                if (nr != (ssize_t)strlen(buf))
                                {
                                        fprintf(stderr, "Couldn't write into the log file\n");
                                        return (0);
                                }

                                // In child
                                ++sh_i;
                                --ti;
                                continue ;
                        }
                        // In parent
                        // Now we should skip all tokens from this ')' to the corresponding '('
                        cpar_pars_ind = get_par_by_prompt_ind(d, d->tokens[ti].start_pi, CLOSING_PAR);
                        opar_token_ind = get_token_by_prompt_ind(d, d->pars[cpar_pars_ind].first);
                        ti = opar_token_ind - 1;
                        // Let's launch a subshell
                }
                else if (d->tokens[ti].type == OPEN_PAR)
                {
                        // Exit the current subshell
                        while (wait(NULL) > 0) {} // I doubt we need it here...
                        exit(EXIT_SUCCESS);
                }

                --ti;

        } // while (ti >= 0)

        // Wait for all children to finish
        //printf("Waiting for children...\n");
        while (wait(NULL) > 0)
        {
                //fprintf(stdout, "Parent: Children have finished "
                //      "the execution, parent is done\n");
        }
        close(fd_log);
        return (1);
}
*/

/* Accepts the index of a parenthesis in the array of tokens
 * `d->tokens` and returns the index of this parenthesis in
 * `d->pars`. If there is no parenthesis with such a token
 * index in `d->pars`, returns -1
 *
 *     prompt_ind       - index of token in prompt
 *     pars_i           - index in `d->pars`
 * */
t_ll    get_par_by_prompt_ind(t_engine_data *d, size_t prompt_ind, t_par_type ptype)
{
        t_ll    pars_ind;

        pars_ind = 0;
        // while (pars_ind < d->par_cnt)
        while (pars_ind < (t_ll)d->par_cnt)
        {
                // Go through opening-parentheses `d->pars[i].first`
                if (ptype == OPENING_PAR)
                {
                        //if (prompt_ind == d->pars[pars_ind].first)
                        if (prompt_ind == (size_t)d->pars[pars_ind].first)
                                return (pars_ind);
                }
                else if (ptype == CLOSING_PAR)
                {
                        // Go through closing-parentheses `d->pars[i].second`
                        //if (prompt_ind == d->pars[pars_ind].second)
                        if (prompt_ind == (size_t)d->pars[pars_ind].second)
                                return (pars_ind);
                }
                ++pars_ind;
        }
        return (-1);
}

/* Accepts the index of a parenthesis in `d->pars` and
 * returns the index of this parenthesis in the array
 * of tokens `d->tokens`. If there is no parenthesis
 * with such an index in `d->tokens`, returns -1
 *
 *     ti                       - token index
 *     prompt_ind       - index of token in prompt
 * */
t_ll    get_token_by_prompt_ind(t_engine_data *d, size_t prompt_ind)
{
        t_ll    ti;

        ti = 0;
        //while (ti < d->token_cnt)
        while (ti < (t_ll)d->token_cnt)
        {
                if (prompt_ind == d->tokens[ti].start_pi)
                        return (ti);
                ++ti;
        }
        return (-1);
}

int     close_pipes(t_engine_data *d)
{
        size_t  i;

        i = 0;
        while (i < d->pipe_cnt)
        {
                if (close(d->pipes[i][READ_END]) == -1)
                {
                        perror("close()");
                        return 0;
                }
                if (close(d->pipes[i][WRITE_END]) == -1)
                {
                        perror("close()");
                        return 0;
                }
                ++i;
        }
        return 1;
}

/* To test get_par_by_prompt_ind() and get_token_by_prompt_ind() */
void    getters_tester(t_engine_data *d)
{
        size_t          i;
        size_t          prompt_ind;
        int                     ptc; // parenthesis type as char
        int                     choice;
        t_par_type      ptype;

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

void    print_parsed_data(t_engine_data *d)
{
        size_t  i;

        // Let's output the pipes we found
        i = 0;
        printf("\nPipes:\n");
        while (i < d->pipe_cnt)
        {
                printf("%lu: [%d] [%d]\n", i + 1,
                        d->pipes[i][READ_END], d->pipes[i][WRITE_END]);
                ++i;
        }
        printf("\n");

        // Let's output the operands we found
        i = 0;
        printf("\nOperands:\n");
        while (i < d->op_cnt)
        {
                printf("%lu: [%s] [%d] [%d]\n", i + 1,
                        d->ops[i].name, d->ops[i].read_end, d->ops[i].write_end);
                ++i;
        }
        printf("\n");
}

void    print_tokens(t_engine_data *d)
{
        char    format[MAX_FORMAT_STR_LEN];
        size_t  i;

        strncpy(format, "%d\t%s\t%lu\n", MAX_FORMAT_STR_LEN);
        i = 0;
        printf("\nTokens:\n");
        while (i < d->token_cnt)
        {
                if (d->tokens[i].type == OPERAND)
                        printf(format, i, d->tokens[i].op->name, d->tokens[i].start_pi);
                else if (d->tokens[i].type == PIPE)
                        printf(format, i, TOKEN_PIPE, d->tokens[i].start_pi);
                else if (d->tokens[i].type == OPEN_PAR)
                        printf(format, i, TOKEN_OPEN_PAR, d->tokens[i].start_pi);
                else if (d->tokens[i].type == CLOSE_PAR)
                        printf(format, i, TOKEN_CLOSE_PAR, d->tokens[i].start_pi);
                else if (d->tokens[i].type == AND)
                        printf(format, i, TOKEN_AND, d->tokens[i].start_pi);
                else if (d->tokens[i].type == OR)
                        printf(format, i, TOKEN_OR, d->tokens[i].start_pi);
                ++i;
        }
        printf("\n");
}

void    print_parentheses(t_engine_data *d)
{
        size_t  i;

        i = 0;
        printf("\nParentheses:\n");
        printf("#\t(\t)\n");
        while (i < d->par_cnt)
        {
                printf("%lu\t%d\t%d\n", i + 1, d->pars[i].first, d->pars[i].second);
                ++i;
        }
        printf("\n");
}

int     parser_init_dynamic(t_engine_data *d, char *rline_buf)
{
        d->pi = 0;
        d->pipe_cnt = 0;
        d->op_cnt = 0;
        d->opar_num = 0;
        d->opar_cnt = 0;
        d->cpar_cnt = 0;
        d->par_cnt = 0;
        d->token_cnt = 1;

        d->prompt = rline_buf;
        d->max_tokens = 128;

        d->tokens = malloc(sizeof(t_token) * d->max_tokens);
        if (d->tokens == NULL)
                return (0);
        d->pipes = malloc(sizeof(int[2]) * d->max_tokens);
        if (d->pipes == NULL)
        {
                free(d->tokens);
                return (0);
        }
        d->ops = malloc(sizeof(t_operand) * d->max_tokens);
        if (d->ops == NULL)
        {
                free(d->pipes);
                free(d->tokens);
                return (0);
        }
        d->pars = malloc(sizeof(t_pair) * d->max_tokens);
        if (d->pars == NULL)
        {
                free(d->ops);
                free(d->pipes);
                free(d->tokens);
                return (0);
        }
        d->open_par = malloc(sizeof(size_t) * d->max_tokens);
        if (d->open_par == NULL)
        {
                free(d->pars);
                free(d->ops);
                free(d->pipes);
                free(d->tokens);
                return (0);
        }
        d->close_par = malloc(sizeof(size_t[2]) * d->max_tokens);
        if (d->close_par == NULL)
        {
                free(d->open_par);
                free(d->pars);
                free(d->ops);
                free(d->pipes);
                free(d->tokens);
                return (0);
        }
        d->all_open_pars = malloc(sizeof(size_t[2]) * d->max_tokens);
        if (d->all_open_pars == NULL)
        {
                free(d->close_par);
                free(d->open_par);
                free(d->pars);
                free(d->ops);
                free(d->pipes);
                free(d->tokens);
                return (0);
        }

        init_tokens(d);
        init_close_par(d);
        init_open_par(d);
        init_ops(d);
        init_pars(d);

        return (1);
}

void    free_parser_data(t_engine_data *d)
{
        if (d->tokens != NULL)
                free(d->tokens);
        if (d->pipes != NULL)
                free(d->pipes);
        if (d->ops != NULL)
                free(d->ops);
        if (d->pars != NULL)
                free(d->pars);
        if (d->open_par != NULL)
                free(d->open_par);
        if (d->close_par != NULL)
                free(d->close_par);
        if (d->all_open_pars != NULL)
                free(d->all_open_pars);

        d->tokens = NULL;
        d->pipes = NULL;
        d->ops = NULL;
        d->pars = NULL;
        d->open_par = NULL;
        d->close_par = NULL;
        d->all_open_pars = NULL;
}