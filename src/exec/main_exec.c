/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:00:23 by juramos           #+#    #+#             */
/*   Updated: 2024/05/13 09:47:44 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	check_fd_heredocs(t_minishell *data, int *p_fd)
{
	int	i;
	int	fd_in;

	fd_in = 0;
	if (data->cmd_table->n_redirections > 0)
	{
		i = 0;
		while (i < data->cmd_table->n_redirections)
		{
			if (data->cmd_table->redirections[i]->type == HEREDOC)
			{
				close(p_fd[0]);
				fd_in = open(data->cmd_table->hd_file, O_RDONLY);
				if (fd_in == -1)
				{
					g_global.error_num = EXIT_FAILURE;
					exit(EXIT_FAILURE);
				}
			}
			i++;
		}
	}
	if (!fd_in)
		fd_in = p_fd[0];
	return (fd_in);
}

static int	iter_over_cmds(t_minishell *data)
{
	int	p_fd[2];
	int	fd_in;

	fd_in = data->fd_in;
	while (data->cmd_table)
	{
		redirect_all(data);
		if (pipe(p_fd) == -1)
			return (EXIT_FAILURE);
		ft_fork(data, p_fd);
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		if (data->cmd_table->prev)
			close(fd_in);
		fd_in = check_fd_heredocs(data, p_fd);
		data->cmd_table = data->cmd_table->next;
	}
	return (EXIT_SUCCESS);
}

int	executor(t_minishell *data)
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
	else if (iter_over_cmds(data))
		return (EXIT_FAILURE);
	wait_pids(data, n_pipes);
	return (EXIT_SUCCESS);
}
