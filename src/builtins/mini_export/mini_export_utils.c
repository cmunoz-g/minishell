/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:01:55 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/26 12:30:52 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_variable	*get_var_to_mod(t_variable *local_vars, int laps)
{
	t_variable	*it;
	int			i;

	it = local_vars;
	i = 0;
	while (i < laps && it)
	{
		it = it->next;
		i++;
	}
	return (it);
}

int	variable_in_env(t_variable *variable, char **env)
{
	int	name_size;
	int	i;

	name_size = ft_strlen(variable->name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(variable->name, env[i], name_size))
			return (0);
		i++;
	}
	return (1);
}

int	variable_in_env_char(char *variable, char **env)
{
	int	name_size;
	int	i;

	name_size = get_var_size(variable, true);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(variable, env[i], name_size))
			return (0);
		i++;
	}
	return (1);
}

int	get_comp_size(int name_size, char *var)
{
	int	var_size;

	var_size = get_var_size(var, true);
	if (name_size > var_size)
		return (name_size);
	else
		return (var_size);
}
