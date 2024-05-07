/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:22:55 by juramos           #+#    #+#             */
/*   Updated: 2024/05/07 16:27:47 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_signal_vars(void)
{
	g_global.stop_heredoc = 0;
	g_global.in_cmd = 0;
	g_global.in_heredoc = 0;
}

t_minishell	*init(char **envp)
{
	t_minishell	*data;

	data = (t_minishell *)malloc(sizeof(t_minishell));
	data->token_list = NULL;
	data->cmd_table = NULL;
	data->cmd_table = NULL;
	data->line = NULL;
	data->env_vars = ft_arrdup(envp);
	data->local_vars = NULL;
	if (!data->env_vars)
		error(data, "Could not alloc memory for env. vars.");
	data->pwd = ft_strdup(my_getenv("PWD", data->env_vars));
	if (!data->pwd)
		error(data, "Could not alloc memory for pwd");
	data->old_pwd = ft_strdup(my_getenv("OLDPWD", data->env_vars));
	if (!data->old_pwd)
		error(data, "Could not alloc memory for old pwd");
	g_global.error_num = 0;
	init_signal_vars();
	return (data);
}
