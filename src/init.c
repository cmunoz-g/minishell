/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:22:55 by juramos           #+#    #+#             */
/*   Updated: 2024/04/18 18:06:29 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*init(char **envp)
{
	t_minishell	*data;

	data = (t_minishell *)malloc(sizeof(t_minishell));
	// if (!data)
	// 		error ft
	data->token_list = NULL;
	data->cmd_table = NULL;
	data->env_vars = ft_arrdup(envp);
	if (!data->env_vars)
		exit(1);
	data->pwd = ft_strdup(my_getenv("PWD", data->env_vars));
	data->old_pwd = ft_strdup(my_getenv("OLDPWD", data->env_vars));
	return (data);
}
