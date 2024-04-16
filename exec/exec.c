/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:49:59 by juramos           #+#    #+#             */
/*   Updated: 2024/04/16 10:27:44 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	exec_process(t_cmd_table *tbl, char **env)
{
	char	**cmd;
	char	*path;

	cmd = ft_str_arr_join_exec(tbl->cmd, tbl->args, env);
	path = get_path(cmd[0], env);
	if (!path)
	{
		send_to_stderr(cmd[0], NULL, "command not found");
		free_split(cmd);
		exit(1);
	}
	if (execve(path, cmd, env) == -1)
	{
		send_to_stderr(cmd[0], NULL, strerror(errno));
		free_split(cmd);
		exit(1);
	}
}

void	do_pipe(t_cmd_table *tbl, char **envp)
{
	int		p_fd[2];
	pid_t	pid;

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
		print_cmd(tbl);
		handle_cmd(tbl, envp);
		tbl = tbl->next;
	}
}
