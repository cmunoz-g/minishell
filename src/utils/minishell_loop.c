/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:05:18 by juramos           #+#    #+#             */
/*   Updated: 2024/04/17 13:26:58 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_loop(t_minishell *data);
void	create_main_fork(char *line, t_minishell *data);
void	reset_loop(char *line, t_minishell *data);

void	create_main_fork(char *line, t_minishell *data)
{
	t_token		*token_tmp;
	pid_t		pid;
	int			status;

	join_history(line, data, data->env_vars);
	lexer(line, &(data->token_list));
	token_tmp = data->token_list;
	parser(&(data->cmd_table), &(data->token_list));
	clean_token_list(&token_tmp);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
		executor(data->cmd_table, data->env_vars);
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
		create_main_fork(line, data);
		reset_loop(line, data);
	}
}

void	reset_loop(char *line, t_minishell *data)
{

	clean_cmd_table_list(&(data->cmd_table));
	free(line);
	minishell_loop(data);
}
