/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:49:59 by juramos           #+#    #+#             */
/*   Updated: 2024/04/17 12:24:41 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_process(t_cmd_table *tbl, char **env)
{
	char	**cmd;
	char	*path;

	cmd = ft_str_arr_join_exec(tbl->cmd, tbl->args, env);
	path = get_path(cmd[0], env);
	if (!path)
	{
		send_to_stderr(cmd[0], NULL, "command not found");
		free_arr(cmd);
		exit(1);
	}
	if (execve(path, cmd, env) == -1)
	{
		g_global.error_num = errno;
		send_to_stderr(cmd[0], NULL, strerror(errno));
		free_arr(cmd);
		exit(1);
	}
}

void	do_pipe(t_cmd_table *tbl, char **envp)
{
	int		p_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(p_fd) == -1)
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
	{
		close(p_fd[0]);
		if (!(tbl->next->in == HEREDOC))
			dup2(p_fd[1], 1);
		exec_process(tbl, envp);
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

static void	handle_cmd(t_cmd_table *tbl, char **envp)
{
	if (!tbl)
		exit(0);
	if (tbl->n_redirections > 0)
		if (redirect(tbl))
			exit(0);
	if (!tbl->next || tbl->out == TRUNC || tbl->out == APPEND)
		exec_process(tbl, envp);
	else if (tbl->next)
		do_pipe(tbl, envp);
}

void	executor(t_cmd_table *tbl, char **envp)
{
	check_all_heredocs(tbl, envp);
	while (tbl)
	{
		handle_cmd(tbl, envp);
		tbl = tbl->next;
	}
}
