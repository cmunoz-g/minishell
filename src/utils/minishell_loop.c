/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/05/01 10:10:36 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		minishell_loop(t_minishell *data);
static void	create_main_fork(t_minishell *data);
void		reset_loop(t_minishell *data);
static void	parse_data(t_minishell *data);

static void	parse_data(t_minishell *data, bool *err_redir)
{
	t_token		*token_tmp;

	join_history(data->line, data, data->env_vars);
	lexer(data->line, &(data->token_list));
	token_tmp = data->token_list;
	//if (!check_only_redirs(data->token_list)) // hacer
		parser(&(data->cmd_table), &(data->token_list));
//	else
//	{
	//	*err_redir = true;
		//error_redirs(); // hacer
	//}
	print_cmd_table(data->cmd_table);
	exit(0);
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
	//bool	err_redir;
	
	data->line = readline("\e[1;34m""minishell> ""\e[m");
	if (!data->line)
		exit(EXIT_SUCCESS);
	else if (check_spaces(data->line) || ft_strlen(data->line) == 0)
		reset_loop(data);
	parse_data(data, &err_redir);
	local_variables(data);
	//if (!err_redir)
	{
		if (!data->cmd_table->next && check_if_builtin(data->cmd_table->cmd))
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
	minishell_loop(data);
}
