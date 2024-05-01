/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan <juan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:49:59 by juramos           #+#    #+#             */
/*   Updated: 2024/05/01 13:13:17 by juan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_and_exec_if_executable(char **str, t_minishell *data)
{
	if (access(str[0], F_OK) == 0)
	{
		if (execve(str[0], str, data->env_vars) == -1)
		{
			g_global.error_num = errno;
			send_to_stderr(str[0], NULL, strerror(errno));
			free_arr(str);
			exit(EXIT_FAILURE);
		}
	}
}

static void	exec_process(t_minishell *data)
{
	char	**cmd;
	char	*path;
	int		(*builtin_arr)(t_minishell *data);

	builtin_arr = check_if_builtin(data->cmd_table->cmd);
	if (builtin_arr)
		exit(execute_builtin(data, builtin_arr));
	cmd = ft_str_arr_join_exec(data->cmd_table->cmd,
			data->cmd_table->args, data);
	check_and_exec_if_executable(cmd, data);
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

static void	do_pipe(t_minishell *data)
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
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_global.error_num = WEXITSTATUS(status);
	close(p_fd[1]);
	dup2(p_fd[0], 0);
}

static void	handle_cmd(t_minishell *data)
{
	if (!data->cmd_table)
		exit(EXIT_SUCCESS);
	if (data->cmd_table->n_redirections > 0)
		if (redirect(data->cmd_table, 0))
			exit(EXIT_FAILURE);
	if (!data->cmd_table->next || data->cmd_table->out == TRUNC
		|| data->cmd_table->out == APPEND)
		exec_process(data);
	else if (data->cmd_table->next)
		do_pipe(data);
}

void	executor(t_minishell *data)
{
	check_all_heredocs(data);
	while (data->cmd_table)
	{
		handle_cmd(data);
		data->cmd_table = data->cmd_table->next;
	}
}
