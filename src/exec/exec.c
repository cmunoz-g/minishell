/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:49:59 by juramos           #+#    #+#             */
/*   Updated: 2024/05/09 11:19:09 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_and_exec_if_executable(char **str, t_minishell *data);
static void	do_pipe(t_minishell *data);
static void	exec_process(t_minishell *data);
static void	handle_cmd(t_minishell *data);

static void	check_and_exec_if_executable(char **str, t_minishell *data)
{
	struct stat	path_stat;

	if (!str[0] || !ft_strlen(str[0]))
		exit(EXIT_SUCCESS);
	stat(str[0], &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		send_to_stderr(str[0], NULL, "Is a directory", 0);
		g_global.error_num = 126;
		exit(126);
	}
	if (access(str[0], F_OK) == 0)
	{
		if (execve(str[0], str, data->env_vars) == -1)
		{
			g_global.error_num = errno;
			send_to_stderr(str[0], NULL, "command not found", 0);
			free_arr(str);
			exit(errno);
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
		g_global.error_num = errno;
		send_to_stderr(cmd[0], NULL, "command not found", 1);
		free_arr(cmd);
		exit(127);
	}
	if (execve(path, cmd, data->env_vars) == -1)
	{
		g_global.error_num = errno;
		send_to_stderr(cmd[0], NULL, strerror(errno), 0);
		free_arr(cmd);
		exit(errno);
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
	int	ret;

	if (!data->cmd_table)
		exit(EXIT_SUCCESS);
	if (data->cmd_table->n_redirections > 0)
	{
		ret = redirect(data->cmd_table, 0);
		if (ret)
		{
			g_global.error_num = ret;
			exit(ret);
		}
	}
	if (!data->cmd_table->cmd)
		exit(EXIT_SUCCESS);
	if (!data->cmd_table->next || data->cmd_table->out == TRUNC
		|| data->cmd_table->out == APPEND)
		exec_process(data);
	else if (data->cmd_table->next)
		do_pipe(data);
}

int	executor(t_minishell *data)
{
	if (check_all_heredocs(data))
		return (EXIT_FAILURE);
	while (data->cmd_table)
	{
		handle_cmd(data);
		data->cmd_table = data->cmd_table->next;
	}
	return (EXIT_SUCCESS);
}
