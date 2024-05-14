/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:07 by juramos           #+#    #+#             */
/*   Updated: 2024/05/14 09:55:47 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler(int sig);
static void	sigsegv_handler(int sig);

int	event(void)
{
	return (EXIT_SUCCESS);
}

static void	sigint_handler(int sig)
{
	if (!g_global.in_heredoc)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		g_global.error_num = 128 + sig;
	}
	if (g_global.in_cmd)
	{
		g_global.stop_heredoc = 1;
		g_global.error_num = 128 + sig;
		rl_redisplay();
		rl_done = 1;
		return ;
	}
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	g_global.error_num = 128 + sig;
}

static void	sigsegv_handler(int sig)
{
	g_global.error_num = 128 + sig;
}

void	init_signals(void)
{
	rl_event_hook = event;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGSEGV, sigsegv_handler);
	signal(SIGINT, sigint_handler);
}
