/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:59:41 by juramos           #+#    #+#             */
/*   Updated: 2024/04/15 12:05:26 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

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

# define HEREDOC_MSG "\033[1;34m> \033[0m"

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
	char				*hd_file;
	int					in;
	int					out;
	int					err;
	t_token				*redirections;
	int					n_redirections;
	struct s_cmd_table	*prev;
	struct s_cmd_table	*next;
}						t_cmd_table;

/*	exec  */
void	do_pipe(t_cmd_table *tbl, char **envp);
void	exec_process(t_cmd_table *tbl, char **env);
/*	error_handlers  */
void	send_to_stderr(char *co, char *ar, char *err);
/*	utils  */
int		open_file(char *name, int to_write);
char	*get_path(char *cmd, char **env);
void	free_split(char **arr);
char	*my_getenv(char *key, char **env);
/*	redirections */
int		redirect(t_cmd_table *tbl);
/*	exec_utils */
char	**ft_str_arr_join_exec(char *s1, char **strarr, char **envp);
/*	heredoc */
int		check_all_heredocs(t_cmd_table *tbl, char **envp);
/*	expand */
char	*expand(char *str, int is_heredoc, char **envp);

#endif