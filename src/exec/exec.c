/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:49:59 by juramos           #+#    #+#             */
/*   Updated: 2024/04/18 12:42:24 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_process(t_minishell *data)
{
	char	**cmd;
	char	*path;

	// if (check_if_builtin(tbl->cmd))
	// 	execute_builtin(tbl, env);
	cmd = ft_str_arr_join_exec(data->cmd_table->cmd,
			data->cmd_table->args, data->env_vars);
	path = get_path(cmd[0], data->env_vars);
	if (!path)
	{
		g_global.error_num = 127;
		send_to_stderr(cmd[0], NULL, "command not found");
		free_arr(cmd);
		exit(127);
	}
	if (execve(path, cmd, data->env_vars) == -1)
	{
		g_global.error_num = errno;
		send_to_stderr(cmd[0], NULL, strerror(errno));
		free_arr(cmd);
		exit(EXIT_FAILURE);
	}
}

void	do_pipe(t_minishell *data)
{
	int		p_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(p_fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		close(p_fd[0]);
		if (!(data->cmd_table->next->in == HEREDOC))
			dup2(p_fd[1], 1);
		exec_process(data);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_global.error_num = WEXITSTATUS(status);
		close(p_fd[1]);
		dup2(p_fd[0], 0);
	}
}

static void	handle_cmd(t_minishell *data)
{
	if (!data->cmd_table)
		exit(EXIT_SUCCESS);
	if (data->cmd_table->n_redirections > 0)
		if (redirect(data->cmd_table))
			exit(EXIT_FAILURE);
	if (!data->cmd_table->next || data->cmd_table->out == TRUNC
		|| data->cmd_table->out == APPEND)
		exec_process(data);
	else if (data->cmd_table->next)
		do_pipe(data);
}

void	executor(t_minishell *data)
{
	check_all_heredocs(data->cmd_table, data->env_vars);
	while (data->cmd_table)
	{
		handle_cmd(data);
		data->cmd_table = data->cmd_table->next;
	}
}
