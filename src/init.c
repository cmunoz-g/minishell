/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:22:55 by juramos           #+#    #+#             */
/*   Updated: 2024/04/18 13:25:35 by juramos          ###   ########.fr       */
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
	data->pwd = getcwd(NULL, sizeof(NULL));
	data->env_vars = ft_arrdup(envp);
	if (!data->env_vars)
		exit(1);
	return (data);
}
