#include "minishell.h"

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_flag = 130;
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
		g_flag = 130;
	else if (signal == SIGQUIT)
	{
		write(1, "Quit: 3\n", 9);
		g_flag = 131;
	}
}	


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