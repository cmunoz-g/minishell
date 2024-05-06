/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:07 by juramos           #+#    #+#             */
/*   Updated: 2024/05/06 13:35:11 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	event(void)
{
	return (EXIT_SUCCESS);
}

void	init_signal_vars(void)
{
	g_global.stop_heredoc = 0;
	g_global.in_cmd = 0;
	g_global.in_heredoc = 0;
}

static void	sigint_handler(int sig)
{
	if (!g_global.in_heredoc)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		g_global.error_num = 130;
	}
	if (g_global.in_cmd)
	{
		g_global.stop_heredoc = 1;
		g_global.error_num = 130;
		rl_redisplay();
		rl_done = 1;
		return ;
	}
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void) sig;
}

void	sigquit_handler(int sig)
{
	ft_putendl_fd("Quit", STDERR_FILENO);
	g_global.error_num = 131;
	(void) sig;
}

void	init_signals(void)
{
	rl_event_hook = event;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_handler);
}
