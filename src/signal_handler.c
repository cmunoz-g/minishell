/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:07 by juramos           #+#    #+#             */
/*   Updated: 2024/05/02 15:02:54 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_signal_vars(void)
{
	g_global.stop_heredoc = 0;
	g_global.in_cmd = 0;
}

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_global.error_num = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signal == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signal_handler_child(int signal)
{
	if (signal == SIGINT)
		g_global.error_num = 130;
	else if (signal == SIGQUIT)
	{
		write(1, "Quit: 3\n", 9);
		g_global.error_num = 131;
	}
}

/*
	All of this will be changed
*/
void	signals(bool child_process)
{
	struct sigaction sa;

	if (child_process)
		sa.sa_handler = &signal_handler_child;
	else
		sa.sa_handler = &signal_handler;

	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}