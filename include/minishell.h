/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:09:45 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/26 12:55:04 by camunozg         ###   ########.fr       */
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
# define EXPAND_CLEANER " !#%%&()*+,-./:;<=>?¿@[]^{}~"

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

typedef struct s_minishell	t_minishell;
extern struct s_global		g_global;

typedef struct s_quotes
{
	bool	qt;
	char	qtt;
}				t_quotes;

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
	bool			variable;
	t_minishell		*data;
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
	char		**export_vars;
	char		*pwd;
	pid_t		*pids;
	int			fd_in;
	int			fd_out;
	int			sh_lvl;
	char		*old_pwd;
}				t_minishell;

typedef struct s_global
{
	int	error_num;
	int	stop_heredoc;
	int	in_cmd;
	int	in_heredoc;
	int	reset_pipes;
}				t_global;

//	main
int			main(int argc, char **argv, char **envp);

//	minishell_loop
void		minishell_loop(t_minishell *data);
void		reset_loop(t_minishell *data);

//	init
t_minishell	*init(char **envp);
int			get_sh_lvl(char **envp);
void		init_signal_vars(void);

//	signals
int			event(void);
void		init_signals(void);
void		sigquit_handler(int sig);

//	history
char		*get_home(char **envp);
void		get_past_history(char **envp, t_minishell *data);
void		join_history(char *cmd, t_minishell *data, char **envp);
void		load_history(int fd);

/***							BUILTINS							***/

/*	mini_export	*/
//	mini_export_dec
void		declaration(t_minishell *data, int i, int laps, char **new_env);
void		declaration_aux(t_minishell *data, int laps, int i);
void		declaration_end(t_minishell *data,
				char **new_env, char **new_export);
void		declaration_new(t_minishell *data,
				int i, char ***new_export, char ***new_env);
void		declaration_new_aux(t_minishell *data,
				int i, char ***new_export, char ***new_env);

//	mini_export_operations
char		**add_variable(char *variable, char **env, t_minishell *data);
char		**mod_var(char **env, t_minishell *data, char *variable);
char		**mod_var_export(char **export_vars,
				t_minishell *data, char *variable);
void		mod_var_aux(t_minishell *data,
				char **new_env, int *n_v_i, char *var);

//	mini_export_order
int			partition(char *arr[], int low, int high);
void		env_order(t_minishell *data);
void		env_order_aux(int i, int *j, char **sorted_env);
void		quicksort(char **arr, int low, int high);
void		swap(char **x, char **y);

//	mini_export_quotes
char		*take_out_quotes(char **args, int i, t_minishell *data);
int			quote_in_middle(char *arg, char quote_type);
int			get_new_args_nbr(char **args);
void		check_possible_quotes(t_minishell *data);

//	mini_export_quotes_aux
char		**check_pq_init(int *i_j_w, t_minishell *data);
void		check_pq_aux(t_minishell *data,
				int *i_j_w, char **new_args, char **tmp);
void		check_pq_end(t_minishell *data, char **new_args, int *i_j_w);
void		check_pq_loop(int *i_j_w, t_minishell *data, char **tmp);
void		check_pq_loop_aux(int *i_j_w, t_minishell *data, char **tmp);

//	mini_export_utils
int			get_comp_size(int name_size, char *var);
int			variable_in_env(t_variable *variable, char **env);
int			variable_in_env_char(char *variable, char **env);
t_variable	*get_var_to_mod(t_variable *local_vars, int laps);

//	mini_export_utils_aux
int			check_all_numbers(char *arg);
int			check_if_declaration(char *arg);
void		check_if_declaration_aux(char *arg, bool flag);
void		no_declaration_aux(t_minishell *data, int i, char **new_export);

//	mini_export
char		*get_new_var(char *variable,
				t_variable *local_vars, t_minishell *data);
char		**add_var_no_value(char *variable, char **exp, t_minishell *data);
int			mini_export(t_minishell *data);
void		no_declaration(t_minishell *data,
				int i, char *new_var, char **new_env);
void		reset_export(t_minishell *data, int i, int *declaration, int *laps);
/*				*/

/*	mini_unset	*/
//	mini_unset_utils
int			get_equal_sign(char *variable);
int			get_nbr_env(char **env);
int			is_env(char *variable, char **env);

//	mini_unset
char		**unset_env(char *variable, char **env, t_minishell *data);
int			mini_unset(t_minishell *data);
t_variable	*get_var_to_mod_u(char *variable, t_variable **local_vars);
void		unset_local(char *char_variable, t_variable **local_vars);
/*				*/

//	mini_cd
int			mini_cd(t_minishell	*data);

//	mini_echo
int			mini_echo(t_minishell *data);

//	mini_env
int			mini_env(t_minishell *data);

//	mini_exit
int			check_if_number(char *arg);
int			check_if_too_big(char *arg);
int			mini_exit(t_minishell *data);
long long	ft_atoll(char *str);
void		mini_exit_aux(t_minishell *data);

//	mini_pwd
int			mini_pwd(t_minishell *data);

//	utils_builtins
int			(*check_if_builtin(char *str))(t_minishell *data);
int			execute_builtin(t_minishell *data,
				int (*builtin_arr)(t_minishell *data));
void		run_builtin(t_minishell *data);
void		simple_builtin_executor(t_minishell *data);

/***							EXEC						***/
//	arr_utils
char		**ft_arrdup(char **arr);
char		**ft_str_arr_join_exec(char *s1, char **strarr, t_minishell *data);
void		free_arr(char **arr);

//	error_handlers
void		send_to_stderr(char *co, char *ar, char *err, int is_out);

//	exec_utils
int			get_n_of_pipes(t_minishell *data);
char		*get_path(char *cmd, char **env);
char		*my_getenv(char *key, char **env);
int			open_file(char *name, int to_write);

//	exec
int			single_cmd(t_minishell *data);
void		ft_fork(t_minishell *data, int *p_fd);

// main_exec
int			executor(t_minishell *data);

//	expand
char		*expand(char *str, int is_heredoc, t_minishell *data);

// expand_utils
int			pass_over_expand(char c);

//	heredoc
int			check_all_heredocs(t_minishell *data);

//	redirections
int			redirect(t_cmd_table *tbl, int is_builtin);
int			redirect_all(t_minishell *data);

/***							LEXER							***/
//	lexer_checks
int			check_first_cmd(t_token *it_variable);
int			check_if_pipe(t_token *it_variable);
int			check_if_repeated_value(t_token *it_variable);
int			check_if_variable(t_token *it_variable);
int			is_there_space(t_token *it_variable, char *line);

//	lexer_token_checks
void		check_local_var(t_token **token_list, char *line);
void		check_local_var_aux(char *line, t_token **it_variable);
void		check_redirections(t_token **token_list);

//	lexer_utils
int			is_quoted_var(char *value);
int			is_pos_variable(char *value);
void		init_lexer(bool *qt, int *i, int *start);
void		set_qt(bool *quotes, char *quote_type, char *cmd_line, int i);

//	lexer
void		lexer(char *line, t_token **list);
void		lexer_qt(t_token **token_list, char *cmd_line, int *start, int i);
void		lexer_new_token(t_token **list, char *line, int *start, int *i);
void		lexer_new_token_aux(t_token **list, char *line, int *start, int *i);

//	tokens
t_token		*get_last_token(t_token *token_list);
void		add_token(t_token **token_list, char *cmd_line, int start, int end);
void		get_token_type(t_token *token);

/***							PARSER							***/
//	cmd_table_utils
void		assign_redir_cmd_t(t_token *list, t_cmd_table **table,
				int *w, char r);
void		assign_redir_cmd_table_aux(t_cmd_table **table, int *w,
				int t, char *v);
void		check_std_cmd_table(t_token *token_list,
				t_cmd_table **cmd_table, int w);
void		check_redir_cmd_table(t_token *token_list,
				char *redir);
void		check_redir_cmd_table_aux(t_token *token_list,
				char *redir);

//	cmd_table
void		gen_cmd_table(t_token *list, t_cmd_table **table,
				int start, int end);
void		gen_cmd_table_redir(t_cmd_table *last,
				t_minishell *data);
void		init_cmd_table(t_cmd_table **cmd_table);
void		init_pop_cmd_table(int *i_j, int *w, char *redir,
				t_cmd_table **table);
void		populate_cmd_table(t_token *list, t_cmd_table **cmd_table,
				int nbr_tok);

//	parser_utils
int			get_nbr_args(t_token *token_list, int nbr_tokens);
int			get_nbr_redir(t_token *token_list, int nbr_tokens);
t_cmd_table	*get_last_cmd_table(t_cmd_table *cmd_list);

//	parser
int			parser_conditions(t_token *token);
void		alloc_cmd_table(t_cmd_table **cmd_list);
void		init_parser(int start_end[2], t_token **tmp, t_token **token_list);
void		parser(t_cmd_table **cmd_table, t_token **token_list);
void		parser_aux(t_token **token_list, int *s_e);

/***							SYNTAX							***/
//	comments_end
int			check_comments(t_token **token_list);
void		take_out_tokens(t_token **tmp);
void		check_end(t_token **token_list);
int			check_only_quote(t_token *token_list);

//	syntax
int			check_syntax(t_token *token_list);
int			check_syntax_end(t_token *it);
int			check_syntax_pipe(t_token *it);
int			check_syntax_redir(t_token *it);
void		print_syntax_error(t_token *token);

/*** 							UTILS 							***/
//	clean_cmd
t_cmd_table	*get_first_node(t_cmd_table *cmd);
void		clean_cmd_table_list(t_cmd_table *cmd_table);

//	clean
void		clean_data(t_minishell **data);
void		clean_local_vars(t_variable **local_vars);
void		clean_token_list(t_token **token_list);
void		get_first_variable(t_variable **var);

//	error
int			error_builtins(t_minishell *data, int exit_code);
void		error(t_minishell *data, char *error_message);

//	extra_libft
char		*ft_strdup_mod(const char *s, size_t size);
int			check_spaces(char *line);
int			ft_strcmp(const char *str1, const char *str2);

/***							VARIABLES							***/
//	local_variables_checks
int			check_new_var(char *cmd, t_variable *local_vars);
int			check_variable(t_cmd_table *cmd_table);

//	local_variablers_quotes
int			get_new_var_space(t_cmd_table *tmp);
void		init_var_quotes(int *i_j_w,
				t_cmd_table **tmp, int *ns, t_quotes *qt);
void		set_quote_var_space(t_quotes *quote,
				t_cmd_table *tmp, int *i_j);
void		variable_with_quotes(t_cmd_table **tmp, t_minishell *data);
void		var_quotes_loop(t_cmd_table **tmp,
				int *i_j_w, t_quotes *qt, char *n);

//	local_variables_utils
int			get_var_size(char *cmd, bool name);
t_variable	*get_last_variable(t_variable *local_vars);
void		create_new_variable(char *cmd, t_variable **loc, t_minishell *data);
void		fill_variable(t_variable **variables, char *cmd, t_minishell *data);

//	local_variables_utils_aux
void		clean_table_var(t_cmd_table **tmp,
				t_cmd_table **next, t_minishell *data);
void		cmd_table_no_vars(t_minishell *data);
void		init_get_new_var_space(int *res, int *i_j, t_quotes *quote);

//	local_variables
int			break_variables_loop(t_minishell *data);
void		add_local_variables(char **new_env,
				char **new_export, t_minishell *data, t_cmd_table *tmp);
void		change_var_value(char *cmd,
				t_variable **loc, int l, t_minishell *data);
void		local_variables(t_minishell *data);
void		local_variables_aux(t_minishell *data, t_cmd_table *tmp);

//	sh_lvl
int			get_nbr_digits(int sh_lvl);
void		modify_sh_lvl(char *tmp, t_minishell *data);
void		modify_sh_lvl_export(char *tmp, t_minishell *data);
void		update_sh_lvl(t_minishell *data, int operation);

#endif