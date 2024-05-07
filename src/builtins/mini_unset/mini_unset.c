/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:14:36 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 16:17:27 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**unset_env(char *variable, char **env, t_minishell *data)
{
	char	**new_env;
	int		nbr_env;
	int		i;
	int		j;
	int		equal_pos;

	i = -1;
	j = 0;
	nbr_env = get_nbr_env(env);
	new_env = (char **)malloc(sizeof(char *) * (nbr_env));
	if (!new_env)
		error(data, "Memory problems in mini_unset");
	while (++i < nbr_env && env[i])
	{
		equal_pos = get_equal_sign(env[i]);
		if (ft_strncmp(variable, env[i], equal_pos))
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j++])
				(free_arr(new_env),
					error(data, "Memory problems in mini_unset"));
		}
	}
	new_env[j] = NULL;
	return (new_env);
}

t_variable	*get_var_to_mod_u(char *variable, t_variable **local_vars)
{
	int			name_size;
	t_variable	*it;

	name_size = get_var_size(variable, true);
	it = (*local_vars);
	while (it)
	{
		if (!ft_strncmp(variable, it->name, name_size))
			return (it);
		it = it->next;
	}
	return (NULL);
}

void	unset_local(char *char_variable, t_variable **local_vars)
{
	t_variable	*variable;

	variable = get_var_to_mod_u(char_variable, local_vars);
	if (!variable)
		return ;
	if (variable->next)
		variable->next->prev = variable->prev;
	if (variable->prev)
		variable->prev->next = variable->next;
	else
		*local_vars = variable->next;
	if (variable->name)
		free(variable->name);
	variable->name = NULL;
	if (variable->value)
		free(variable->value);
	variable->value = NULL;
	free(variable);
	variable = NULL;
}

int	mini_unset(t_minishell *data)
{
	int		i;
	char	**new_env;

	i = 0;
	while (data->cmd_table->args[i])
	{
		if (!is_env(data->cmd_table->args[i], data->env_vars))
		{
			new_env = unset_env(data->cmd_table->args[i], data->env_vars, data);
			free_arr(data->env_vars);
			data->env_vars = new_env;
		}
		unset_local(data->cmd_table->args[i], &(data->local_vars));
		i++;
	}
	return (0);
}
