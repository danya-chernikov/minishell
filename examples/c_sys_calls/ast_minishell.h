#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

#if 0
ENUMS

Identificadores para el Lexer (Tokens crudos)
#endif
typedef enum	e_token_type
{
	T_WORD,			// Comandos, argumentos, nombres de archivo 
	T_PIPE,			// |
	T_AND,			// &&
	T_OR,			// ||
	T_LEFT_PAREN,		// (
	T_RIGHT_PAREN,		// )
	T_REDIR_IN,		// <
	T_REDIR_OUT,		// >
	T_APPEND,		// >>
	T_HEREDOC		// <<
}	t_token_type;

#if 0
Identificadores para el Parser (Nodos del AST)
#endif
typedef enum	e_node_type
{
	NODE_PIPE,		// Nodo de control: Pipe
	NODE_AND,		// Nodo de control: AND
	NODE_OR,		// Nodo de control: OR
	NODE_CMD,		// Nodo hoja: Comando simple
	NODE_SUBSHELL		// Nodo hoja especial: (...)
}	t_node_type;

#if 0
Structura principal
Alamacenara el esta global de nuestra minishell
#endif
typedef struct	s_minishell
{
	char	**env;		// Variables de entorno (para execve) 
	int	status;		// $? (para la lógica && y ||)
}	t_minishell;

#if 0
Lista de Tokens
Salida plana antes de contruir el arbol
#endif
typedef struct	s_token
{
	t_token_type	type;
	char		*value;	// El texto real ("ls", "|", "&&")
	struct s_token	*next;
	struct s_token	*prev;	// Para mirar atrás al parsear
}	t_token;

#if 0
Lista de Redirecciones
Se adjunta a cada comando. Se procesara de izquierda a derecha
#endif
typedef struct	s_redi_node
{
	t_token_type		type;		// T_REDIR_IN, T_APPEND,etc.
	char			*filename;	// Nombre del archivo o delimitador heredoc
	struct s_redi_node	*next;
}	t_redi_node;

#if 0
Nodos del AST
Una estrucutra unificada para cualquier nodo del arbol
#endif
typedef struct	s_ast
{
	t_node_type	type;		// (PIPE, AND, CMD...)
	struct s_ast	*left;
	struct s_ast	*right;
	char		**args;		// argv para execve: ["ls", "-la", NULL]
	t_redi_node	*redirections;	// Lista lineal de <, >, <<, >>
}	t_ast;

#endif
