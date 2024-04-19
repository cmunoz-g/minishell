/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 13:52:02 by juramos           #+#    #+#             */
/*   Updated: 2024/04/19 14:01:34 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_pwd(t_minishell *data)
{
	if (data->cmd_table->n_args > 0)
	{
		printf("minishell: pwd: too many arguments\n");
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(data->pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
