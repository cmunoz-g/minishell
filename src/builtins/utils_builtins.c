/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:50:59 by juramos           #+#    #+#             */
/*   Updated: 2024/05/08 17:16:00 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	(*check_if_builtin(char *str))(t_minishell *data);

void	run_builtin(t_minishell *data)
{
	int		ret;
	int		(*builtin_arr)(t_minishell *data);

	builtin_arr = check_if_builtin(data->cmd_table->cmd);
	ret = builtin_arr(data);
	if (ret)
		g_global.error_num = ret;
}

void	simple_builtin_executor(t_minishell *data)
{
	int		ret;
	int		in;
	int		out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	if (check_all_heredocs(data))
		g_global.error_num = 130;
	if (data->cmd_table->n_redirections > 0)
	{
		ret = redirect(data->cmd_table, 1);
		if (ret)
		{
			g_global.error_num = ret;
			return ;
		}
	}
	if (!g_global.stop_heredoc)
		run_builtin(data);
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
}

int	execute_builtin(t_minishell *data, int (*builtin_arr)(t_minishell *data))
{
	int	ret;

	ret = builtin_arr(data);
	if (ret)
	{
		g_global.error_num = ret;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	(*check_if_builtin(char *str))(t_minishell *data)
{
	static void	*builtins[7][2] = {
	{"echo", mini_echo},
	{"cd", mini_cd},
	{"pwd", mini_pwd},
	{"export", mini_export},
	{"unset", mini_unset},
	{"env", mini_env},
	{"exit", mini_exit},
	};
	int			i;

	i = 0;
	while (i < 7)
	{
		if (str)
		{
			if (!ft_strncmp(builtins[i][0], str, ft_strlen(str))
				&& !ft_strncmp(builtins[i][0], str, ft_strlen(builtins[i][0])))
				return (builtins[i][1]);
		}
		i++;
	}
	return (NULL);
}
