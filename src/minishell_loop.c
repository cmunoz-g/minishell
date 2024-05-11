/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:41:43 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/11 13:50:33 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	executor(t_minishell *data);
static void	parse_data(t_minishell *data, bool *err_syntax);
static void	print_exit_msg(int wstatus, int signo);

void	print_cmd_table(t_cmd_table *cmd_table) // borrar
{
	int i = 0; 

	while (cmd_table) 
	{
		printf("CMD:%s\n",cmd_table->cmd);
		if (cmd_table->args)
		{
			while (cmd_table->args[i])
			{
				printf("ARG%d:%s\n", i, cmd_table->args[i]);
				i++;
			}

		}
		if (cmd_table->in)
			printf("IN:%d\n", cmd_table->in);
		printf("OUT:%d\n", cmd_table->out);
		printf("ERR:%d\n", cmd_table->err);
		i = 0;
		while (i < cmd_table->n_redirections)
		{
			printf("redir number: %d type: %d value: %s\n", i, cmd_table->redirections[i]->type, cmd_table->redirections[i]->value);
			i++;
		}
		printf("nbr redir: %d\n", cmd_table->n_redirections);
		// if (cmd_table->new_cmd)
		// 	printf("new cdm TRUE\n");
		// else
		// 	printf("new cdm FALSE\n");
		cmd_table = cmd_table->next;
		printf("\n");
	}
}

void	print_tokens(t_token *token_list) // borrar
{
	while (token_list)
	{
	 	printf("value:%s type:%d\n",token_list->value, token_list->type);
	 	token_list = token_list->next;
	}
}

static void	parse_data(t_minishell *data, bool *err_syntax)
{
	t_token	*token_tmp;

	join_history(data->line, data, data->env_vars);
	lexer(data->line, &(data->token_list));
	data->token_list->data = data;
	token_tmp = data->token_list;
	if (!check_comments(&(data->token_list)))
	{
		*err_syntax = true;
		data->token_list = NULL;
	}
	else
	{
		if (!check_syntax(data->token_list))
			parser(&(data->cmd_table), &(data->token_list));
		else
			*err_syntax = true;
	}
	clean_token_list(&token_tmp);
}

static void	print_exit_msg(int wstatus, int signo)
{
	if (signo == SIGQUIT)
		ft_putstr_fd("Quit", STDERR_FILENO);
	else if (signo == SIGSEGV)
		ft_putstr_fd("Segmentation fault", STDERR_FILENO);
	if (WCOREDUMP(wstatus))
		ft_putstr_fd(" (core dumped)", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

static void	wait_pids(t_minishell *data, int n_pipes)
{
	int	status;
	int	i;
	int	signo;

	i = 0;
	while (i <= n_pipes)
	{
		waitpid(data->pids[i], &status, 0);
		if (WIFEXITED(status))
			g_global.error_num = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			signo = WTERMSIG(status);
			if (signo == SIGQUIT || signo == SIGSEGV)
				print_exit_msg(status, signo);
		}
		i++;
	}
}

static int	executor(t_minishell *data)
{
	int	n_pipes;

	n_pipes = get_n_of_pipes(data);
	data->pids = ft_calloc(n_pipes + 2, sizeof(pid_t *));
	if (!data->pids)
		return (EXIT_FAILURE);
	if (check_all_heredocs(data))
		return (EXIT_FAILURE);
	if (n_pipes == 0)
		return (single_cmd(data));
	while (data->cmd_table)
	{
		redirect_all(data);
		ft_fork(data);
		data->cmd_table = data->cmd_table->next;
	}
	wait_pids(data, n_pipes);
	return (EXIT_SUCCESS);
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
		signal(SIGQUIT, sigquit_handler);
		if (!check_variable(data->cmd_table))
			reset_loop(data);
		g_global.in_cmd = 1;
		if (!data->cmd_table->next
			&& check_if_builtin(data->cmd_table->cmd))
			simple_builtin_executor(data);
		else
			executor(data);
		g_global.in_cmd = 0;
	}
	reset_loop(data);
}

void	reset_loop(t_minishell *data)
{
	if (data->pids)
	{
		free(data->pids);
		data->pids = NULL;
	}
	if (data->cmd_table)
		clean_cmd_table_list(&(data->cmd_table));
	if (data->line || ft_strlen(data->line))
		free(data->line);
	if (data->token_list)
		data->token_list = NULL;
	init_signal_vars();
	dup2(data->fd_in, STDIN_FILENO);
	dup2(data->fd_out, STDOUT_FILENO);
	minishell_loop(data);
}
