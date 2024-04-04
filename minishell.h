#ifndef MINISHELL_H
#define MINISHELL_H

typedef enum	e_token_type
{
	TOK_CMD,
	TOK_ARG,
	TOK_OPR;
}				t_token_type;

typedef struct	s_token;
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}				t_token;

#endif