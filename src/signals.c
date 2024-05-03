/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:07 by juramos           #+#    #+#             */
/*   Updated: 2024/05/03 11:35:18 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	event(void)
{
	return (EXIT_SUCCESS);
}

void		init_signal_vars(void)
{
	g_global.stop_heredoc = 0;
	g_global.in_cmd = 0;
	g_global.in_heredoc = 0;
}

static void	sigint_handler(int sig)
{
	if (!g_global.in_heredoc)
		ft_putstr_fd("\n", STDERR_FILENO);
	if (g_global.in_cmd)
	{	
		g_global.stop_heredoc = 1;
		rl_redisplay();
		rl_done = 1;
		return ;
	}
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void) sig;
}

// static void	sigquit_handler(int sig)
// {
// 	ft_putstr_fd("Quit: ", STDERR_FILENO);
// 	ft_putnbr_fd(sig, STDERR_FILENO);
// 	ft_putchar_fd('\n', STDERR_FILENO);
// }

void	init_signals(void)
{
	rl_event_hook = event;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_handler);
}
