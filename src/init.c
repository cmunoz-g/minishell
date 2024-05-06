/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:22:55 by juramos           #+#    #+#             */
/*   Updated: 2024/05/06 10:27:03 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
/*typedef struct s_minishell
{
	char		*line;
	t_token		*token_list;
	t_cmd_table	*cmd_table;
	t_variable	*local_vars;
	char		**env_vars;
	char		*pwd;
	char		*old_pwd;
}				t_minishell;*/