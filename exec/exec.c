/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:49:59 by juramos           #+#    #+#             */
/*   Updated: 2024/04/11 10:29:44 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	exec_process(t_cmd_table *tbl, char **env)
{
	char	**cmd;
	char	*path;

	cmd = ft_str_arr_join_exec(tbl->cmd, tbl->args);
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

static void	here_doc_ingest(char **argv, int *p_fd)
{
	char	*ret;

	while (1)
	{
		ret = get_next_line(0);
		if (ft_strncmp(ret, argv[2], ft_strlen(argv[2])) == 0)
		{
			free(ret);
			exit(0);
		}
		ft_putstr_fd(ret, p_fd[1]);
		free(ret);
	}
}

void	here_doc(char **argv)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
		here_doc_ingest(argv, p_fd);
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], 0);
		wait(NULL);
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
		dup2(p_fd[1], 1);
		exec_process(tbl, envp);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], 0);
	}
}
