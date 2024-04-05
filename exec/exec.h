/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:59:41 by juramos           #+#    #+#             */
/*   Updated: 2024/04/05 12:02:21 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
#include "../libft/libft.h"
# include <unistd.h>
# include <errno.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/wait.h>

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7
# define STDIN 8
# define FILE 9

typedef struct s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

typedef struct s_cmd_table
{
	char				*cmd;
	char				*arg;
	int					in;
	int					out;
	char				*filename;
	struct s_cmd_table	*prev;
	struct s_cmd_table	*next;
}				t_cmd_table;

/*	exec  */
void	do_pipe(char *cmd, char **envp);
void	here_doc(char **argv);
void	exec_process(char *cmd, char **env);
/*	error_handlers  */
void	send_to_stderr(char *co, char *ar, char *err);
void	exit_handler(void);
/*	utils  */
int		open_file(char *name, int to_write);
char	*get_path(char *cmd, char **env);
void	free_split(char **arr);
#endif