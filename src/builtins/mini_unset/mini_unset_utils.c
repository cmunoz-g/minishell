/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:14:38 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 16:17:42 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_nbr_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

int	get_equal_sign(char *variable)
{
	int	i;

	i = 0;
	while (variable[i] && variable[i] != '=')
		i++;
	return (i);
}

int	is_env(char *variable, char **env)
{
	int	i;
	int	equal_pos;

	i = 0;
	equal_pos = 0;
	while (env[i])
	{
		equal_pos = get_equal_sign(env[i]);
		if (!ft_strncmp(variable, env[i], equal_pos))
			return (0);
		i++;
	}
	return (1);
}
