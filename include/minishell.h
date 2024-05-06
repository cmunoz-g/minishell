/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:27:08 by juramos           #+#    #+#             */
/*   Updated: 2024/05/06 10:25:56 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# define DOUBLEPIPE 13
# define PATH_MAX	4096

# define HEREDOC_MSG "\033[1;34m> \033[0m"

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "libft.h"
# include <unistd.h>
# include <errno.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <signal.h>
# include <termios.h>

typedef struct s_variable
{
	struct s_variable	*next;
	struct s_variable	*prev;
	char				*name;
	char				*value;
}				t_variable;

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
	int					n_args;
	int					in;
	int					out;
	int					err;
	char				*hd_file;
	t_token				**redirections;
	int					n_redirections;		
	bool				new_cmd;		
	struct s_cmd_table	*prev;
	struct s_cmd_table	*next;
}						t_cmd_table;

typedef struct s_minishell
{
	char		*line;
	t_token		*token_list;
	t_cmd_table	*cmd_table;
	t_variable	*local_vars;
	char		**env_vars;
	char		*pwd;
	char		*old_pwd;
}				t_minishell;

typedef struct s_global
{
	int	error_num;
	int	stop_heredoc;
	int	in_cmd;
	int	in_heredoc;
}				t_global;

extern t_global	g_global;

// LEXER 
void		lexer(char *cmd_line, t_token **token_list);
void		lexer_qt(t_token **token_list, char *cmd_line, int *start, int i);
void		lexer_new_token(t_token **token_list,
				char *cmd_line, int *start, int *i);
void		lexer_new_token_aux(t_token **token_list,
				char *cmd_line, int *start, int *i);
void		lexer_new_cmd(t_token **token_list, char *cmd_line, int *i);
void		check_redirections(t_token **token_list);
void		set_qt(bool *quotes, char *quote_type, char *cmd_line, int i);
void		init_lexer(bool *qt, int *i, int *start);
void		add_token(t_token **token_list, char *cmd_line, int start, int end);
void		get_token_type(t_token *token);
t_token		*get_last_token(t_token *token_list);

// SYNTAX
int			check_syntax(t_token *token_list);
void		check_comments(t_token **token_list);
void		take_out_tokens(t_token **tmp);

// PARSER
void		parser(t_cmd_table **cmd_table, t_token **token_list);
void		init_parser(int start_end[2], t_token **tmp,
				t_token **token_list, bool *new_cmd);
void		reset_parser(t_token **tmp, int start_end[2], bool *new_cmd);
void		parser_aux(t_token **token_list, int *s_e);
void		gen_cmd_table(t_token *token_list, t_cmd_table **cmd_table,
				int start, int end);
void		alloc_cmd_table(t_cmd_table **cmd_list, bool new_cmd);
void		populate_cmd_table(t_token *token_list,
				t_cmd_table **cmd_table, int nbr_tokens);
t_cmd_table	*get_last_cmd_table(t_cmd_table *cmd_list);
int			get_nbr_args(t_token *token_list, int nbr_tokens);
int			get_nbr_redir(t_token *token_list, int nbr_tokens);
void		init_pop_cmd_table(int *i, int *j, int *w, char *redir,
				t_cmd_table **cmd_table);
void		check_std_cmd_table(t_token *token_list,
				t_cmd_table **cmd_table, int w);
void		check_redir_cmd_table(t_token *token_list, char *redir);
void		assign_redir_cmd_table(t_token *token_list, t_cmd_table **cmd_table,
				int *w, char redir);
void		assign_redir_cmd_table_aux(t_cmd_table **cmd_table, int *w,
				int type, char *value);

// INIT
t_minishell	*init(char **envp);

//BUILTINS
/* mini_cd */
int			mini_cd(t_minishell	*data);
/* mini_exit */
int			mini_exit(t_minishell *data);
/* mini_pwd */
int			mini_pwd(t_minishell *data);
/* mini_echo */
int			mini_echo(t_minishell *data);
/* mini_env */
int			mini_env(t_minishell *data);
/* mini_unset*/
int			mini_unset(t_minishell *data);
int			get_nbr_env(char **env);
/* mini_export*/
int			mini_export(t_minishell *data);
int			variable_in_env(t_variable *variable, char **env);
t_variable	*get_var_to_mod(t_variable *local_vars, int laps);
/* utills_builtins */
int			(*check_if_builtin(char *str))(t_minishell *data);
void		simple_builtin_executor(t_minishell *data);
int			execute_builtin(t_minishell *data,
				int (*builtin_arr)(t_minishell *data));

// UTILS
void		clean_token_list(t_token **token_list);
void		clean_cmd_table_list(t_cmd_table **cmd_table);
void		clean_cmd_table_redir(t_cmd_table **cmd_table, int *j);
void		clean_data(t_minishell **data);
void		clean_local_vars(t_variable **local_vars);
void		get_first_variable(t_variable **var);
int			ft_strcmp(const char *str1, const char *str2);
int			ft_isspace(int c);
int			check_spaces(char *line);
char		*ft_strdup_mod(const char *s, size_t size);
void		error(t_minishell *data, char *error_message);
int			error_builtins(t_minishell *data, int exit_code);

/*	minishell_loop */
void		minishell_loop(t_minishell *data);
void		reset_loop(t_minishell *data);

/* signals */
void		init_signals(void);
void		init_signal_vars(void);

// HISTORY
void		join_history(char *cmd, t_minishell *data, char **envp);
void		get_past_history(char **envp, t_minishell *data);
void		load_history(int fd);
char		*get_home(char **envp);

// LOCAL VARIABLES
void		local_variables(t_minishell *data);
int			check_variable(t_cmd_table *cmd_table);
void		create_new_variable(char *cmd, t_variable **local_vars,
				t_minishell *data);
void		fill_variable(t_variable **variables, char *cmd, t_minishell *data);
int			get_var_size(char *cmd, bool name);
int			check_new_var(char *cmd, t_variable *local_vars);
void		change_var_value(char *cmd, t_variable **local_vars,
				int laps, t_minishell *data);
t_variable	*get_last_variable(t_variable *local_vars);

// EXEC
/*	exec */
int			executor(t_minishell *data);
/*	error_handlers  */
void		send_to_stderr(char *co, char *ar, char *err, int is_out);
/*	exec_utils  */
int			open_file(char *name, int to_write);
char		*get_path(char *cmd, char **env);
char		*my_getenv(char *key, char **env);
/*	redirections */
int			redirect(t_cmd_table *tbl, int is_builtin);
/*	arr_utils */
char		**ft_str_arr_join_exec(char *s1, char **strarr, t_minishell *data);
void		free_arr(char **arr);
char		**ft_arrdup(char **arr);
/*	heredoc */
int			check_all_heredocs(t_minishell *data);
/*	expand */
char		*expand(char *str, int is_heredoc, t_minishell *data);

// DELETE
void		print_tokens(t_token *token_list);
void		print_cmd_table(t_cmd_table *cmd_table);
void		print_local_variables(t_variable *local_vars);

#endif