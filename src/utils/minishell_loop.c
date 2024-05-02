/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/05/01 13:34:37 by juramos          ###   ########.fr       */
/*   Updated: 2024/05/01 12:17:07 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		minishell_loop(t_minishell *data);
static void	create_main_fork(t_minishell *data);
void		reset_loop(t_minishell *data);
static void	parse_data(t_minishell *data, bool *err_syntax);

static void	parse_data(t_minishell *data, bool *err_syntax)
{
	t_token		*token_tmp;

	join_history(data->line, data, data->env_vars);
	lexer(data->line, &(data->token_list));
	token_tmp = data->token_list;
	if (!check_syntax(data->token_list))
		parser(&(data->cmd_table), &(data->token_list));
	else
		*err_syntax = true;
	clean_token_list(&token_tmp);
}

static void	create_main_fork(t_minishell *data)
{
	pid_t		pid;
	int			status;

	status = 0;
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
		executor(data);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_global.error_num = WEXITSTATUS(status);
}

void	minishell_loop(t_minishell *data)
{
	bool	err_syntax;
	
	err_syntax = false;
	data->line = readline("\e[1;34m""minishell> ""\e[m");
	if (!data->line)
		exit(EXIT_SUCCESS);
	else if (check_spaces(data->line) || ft_strlen(data->line) == 0)
		reset_loop(data);
	parse_data(data, &err_syntax);
	local_variables(data);
	if (!err_syntax)
	{
		if (!check_variable(data->cmd_table))
			reset_loop(data);
		else if (!data->cmd_table->next && check_if_builtin(data->cmd_table->cmd))
			simple_builtin_executor(data);
		else
			create_main_fork(data);
	}
	reset_loop(data);
}

void	reset_loop(t_minishell *data)
{
	if (data->cmd_table)
		clean_cmd_table_list(&(data->cmd_table));
	if (data->line || ft_strlen(data->line)) // porque el ft_strlen? puede que acceda a NULL?
		free(data->line);
	if (data->token_list)
		data->token_list = NULL;
	minishell_loop(data);
}
