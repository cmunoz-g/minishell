/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:15:45 by juramos           #+#    #+#             */
/*   Updated: 2024/04/20 12:48:18 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_echo(t_minishell *data)
{
	int		endl;
	int		i;
	char	*expanded;

	i = 0;
	if (!data->cmd_table->args[0])
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	endl = (ft_strncmp(data->cmd_table->args[0], "-n", 2) == 0);
	if (endl)
		i++;
	while (data->cmd_table->args[i])
	{
		// never freed, when freed fails in scenario
		// where such as echo $USER how u d $PWD ss
		expanded = expand(data->cmd_table->args[i], 0, data->env_vars);
		ft_putstr_fd(expanded, STDOUT_FILENO);
		i++;
		if (data->cmd_table->args[i])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (!endl)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
