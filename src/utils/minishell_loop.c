/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:05:18 by juramos           #+#    #+#             */
/*   Updated: 2024/04/18 17:47:47 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_loop(t_minishell *data);
void	create_main_fork(t_minishell *data);
void	reset_loop(char *line, t_minishell *data);
void	parse_data(char *line, t_minishell *data);

void	parse_data(char *line, t_minishell *data)
{
	t_token		*token_tmp;

	join_history(line, data, data->env_vars);
	lexer(line, &(data->token_list));
	token_tmp = data->token_list;
	parser(&(data->cmd_table), &(data->token_list));
	clean_token_list(&token_tmp);
}

void	create_main_fork(t_minishell *data)
{
	pid_t		pid;
	int			status;

	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
		executor(data);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_global.error_num = WEXITSTATUS(status);
	}
}

void	minishell_loop(t_minishell *data)
{
	char	*line;

	line = readline("\e[1;34m""minishell> ""\e[m");
	if (!line)
		exit(EXIT_SUCCESS);
	else if (check_spaces(line))
		reset_loop(line, data);
	else
	{
		parse_data(line, data);
		create_main_fork(data);
		reset_loop(line, data);
	}
}

void	reset_loop(char *line, t_minishell *data)
{
	clean_cmd_table_list(&(data->cmd_table));
	free(line);
	minishell_loop(data);
}
