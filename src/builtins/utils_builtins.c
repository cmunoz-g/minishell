/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:50:59 by juramos           #+#    #+#             */
/*   Updated: 2024/05/06 12:48:09 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	(*check_if_builtin(char *str))(t_minishell *data);

void	simple_builtin_executor(t_minishell *data)
{
	int		ret;
	int		(*builtin_arr)(t_minishell *data);

	if (check_all_heredocs(data))
		g_global.error_num = 130;
	if (!data->cmd_table)
		exit(EXIT_SUCCESS);
	if (data->cmd_table->n_redirections > 0)
	{
		ret = redirect(data->cmd_table, 1);
		if (ret)
		{
			g_global.error_num = ret;
			exit(ret);
		}
	}
	builtin_arr = check_if_builtin(data->cmd_table->cmd);
	ret = builtin_arr(data);
	if (ret)
		g_global.error_num = ret;
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
			if (!ft_strncmp(builtins[i][0], str, ft_strlen((builtins[i][0]))))
				return (builtins[i][1]);
		}
		i++;
	}
	return (NULL);
}
