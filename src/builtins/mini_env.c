/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 12:31:20 by juramos           #+#    #+#             */
/*   Updated: 2024/04/26 12:33:33 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_env(t_minishell *data)
{
	int	i;

	i = 0;
	while (data->env_vars[i])
	{
		ft_putendl_fd(data->env_vars[i], 1);
		i++;
	}
	return (0);
}
