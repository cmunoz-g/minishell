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
# define STDIN 8
# define STDOUT 9
# define STDERR 10
# define FILENAME 11
# define HEREDOC 12

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "../libft/libft.h"
# include <unistd.h>
# include <errno.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
	struct s_token	*next_cmd;
}					t_token;

typedef struct s_cmd_table
{
	char				*cmd;
	char				**args;
	int					in;
	int					out;
	int 				err;
	char				*hd_file;
	t_token				**redirections;
	int					n_redirections;		
	bool				new_cmd;		
	struct s_cmd_table	*prev;
	struct s_cmd_table	*next;
}						t_cmd_table;

// LEXER 
void		lexer(char *cmd_line, t_token **token_list);
void		lexer_qt(t_token **token_list, char *cmd_line, int *start, int i);
void		lexer_new_token(t_token **token_list, char *cmd_line, int *start, int *i);
void		lexer_new_cmd(t_token **token_list, char *cmd_line, int *i);
void		check_redirections(t_token **token_list);
void		set_qt(bool *quotes, char *quote_type, char *cmd_line, int i);
void		init_lexer(bool *qt, int *i, int *start);
void		add_token(t_token **token_list, char *cmd_line, int start, int end);
void		get_token_type(t_token *token);
t_token 	*get_last_token(t_token *token_list);

// PARSER
void		parser(t_cmd_table **cmd_table, t_token **token_list);
void		init_parser(int start_end[2], t_token **tmp, t_token **token_list, bool *new_cmd);
void		reset_parser(t_token **tmp, int start_end[2], bool *new_cmd);
void		parser_aux(t_token **token_list, int *s_e);
void		gen_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int start, int end);
void		alloc_cmd_table(t_cmd_table **cmd_list, bool new_cmd);
void		populate_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int nbr_tokens);
t_cmd_table *get_last_cmd_table(t_cmd_table *cmd_list);
int			get_nbr_args(t_token *token_list, int nbr_tokens);
int			get_nbr_redir(t_token *token_list, int nbr_tokens);
void		init_pop_cmd_table(int *i, int *j, int *w, char *redir, t_cmd_table **cmd_table);
void		check_std_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int w);
void		check_redir_cmd_table(t_token *token_list, char *redir);
void		assign_redir_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int *w, char redir);
void		assign_redir_cmd_table_aux(t_cmd_table **cmd_table, int *w, int type, char *value);

// UTILS
void	clean_token_list(t_token **token_list);
void	clean_cmd_table_list(t_cmd_table **cmd_table);
void	clean_cmd_table_redir(t_cmd_table **cmd_table, int *j);
int		ft_strcmp(const char *str1, const char *str2);
int		ft_isspace(int c);
char	*ft_strdup_mod(const char *s, size_t size);

#endif