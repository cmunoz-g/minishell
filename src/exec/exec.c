/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:49:59 by juramos           #+#    #+#             */
/*   Updated: 2024/05/13 09:54:34 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_and_exec_if_executable(char **str, t_minishell *data);
static void	handle_cmd(t_minishell *data, int *p_fd);
static void	exec_process(t_minishell *data);

static void	check_and_exec_if_executable(char **str, t_minishell *data)
{
	struct stat	path_stat;

	if (!str[0] || !ft_strlen(str[0]))
		exit(EXIT_SUCCESS);
	if (access(str[0], F_OK) == 0)
	{
		if (execve(str[0], str, data->env_vars) == -1)
		{
			stat(str[0], &path_stat);
			if (S_ISDIR(path_stat.st_mode))
			{
				send_to_stderr(str[0], NULL, "Is a directory", 0);
				g_global.error_num = 126;
				exit(126);
			}
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

int	single_cmd(t_minishell *data)
{
	pid_t	pid;
	int		status;

	if (!data->cmd_table)
		return (EXIT_SUCCESS);
	if (redirect_all(data))
		return (EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		return (EXIT_FAILURE);
	if (!data->cmd_table->cmd)
		exit (EXIT_SUCCESS);
	if (pid == 0)
		exec_process(data);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_global.error_num = WEXITSTATUS(status);
	return (EXIT_SUCCESS);
}

void	ft_fork(t_minishell *data, int *p_fd)
{
	static int	i = 0;

	if (g_global.reset_pipes)
	{
		g_global.reset_pipes = 0;
		i = 0;
	}
	data->pids[i] = fork();
	if (data->pids[i] == -1)
		exit(EXIT_FAILURE);
	if (data->pids[i] == 0)
		handle_cmd(data, p_fd);
	i++;
}

static void	handle_cmd(t_minishell *data, int *p_fd)
{
	close(p_fd[0]);
	if (!data->cmd_table->cmd)
		exit(EXIT_SUCCESS);
	if (!data->cmd_table->next || data->cmd_table->out == TRUNC
		|| data->cmd_table->out == APPEND)
		exec_process(data);
	else if (data->cmd_table->next)
	{
		dup2(p_fd[1], STDOUT_FILENO);
		exec_process(data);
	}
}
