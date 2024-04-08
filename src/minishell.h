#ifndef MINISHELL_H
#define MINISHELL_H

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7
# define FILENAME 8

typedef struct	s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
	struct s_token	*next_cmd;
}				t_token;

typedef struct	s_cmd_table
{
	char	*cmd;
	char	**args;
	int		in;
	int		out;
	int		err;
	struct 	s_cmd_table *next;
	struct 	s_cmd_table *prev;
}				t_cmd_table;

#endif