/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 12:18:34 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 12:18:50 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_var_size(char *cmd, bool name)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	if (name)
	{
		while (cmd[i] && cmd[i] != '=')
			i++;
		return (i);
	}
	else 
	{
		while (cmd[i] && cmd[i] != '=')
			i++;
		i++;
		while (cmd[i])
		{
			i++;
			j++;
		}
		return (j);
	}
		
}

void	fill_variable(t_variable **variables, char *cmd, t_minishell *data)
{
	int	name_size;
	int	value_size;
	
	name_size = get_var_size(cmd, true);
	value_size = get_var_size(cmd, false);

	(*variables)->name = (char *)malloc(sizeof(char) * (name_size + 1));
	if (!(*variables)->name)
		error(data, "Memory errors while filling variable");
	ft_strlcpy((*variables)->name, cmd, name_size + 1);
	(*variables)->value = (char *)malloc(sizeof(char) * (value_size + 1));
	if (!(*variables)->value)
		error(data, "Memory errors while filling variable");
	ft_strlcpy((*variables)->value, cmd + name_size + 1, value_size + 1);
}

t_variable	*get_last_variable(t_variable *local_vars)
{
	if (!local_vars)
		return (0);
	while (local_vars->next)
		local_vars = local_vars->next;
	return (local_vars);
}

void	create_new_variable(char *cmd, t_variable **local_vars, t_minishell *data)
{
	t_variable *last;
	t_variable *variables;

	variables = (t_variable *)malloc(sizeof(t_variable));
	if (!variables)
		error(data, "Memory problems while creating a new variable");
	last = get_last_variable(*local_vars);
	if (!last)
	{
		(*local_vars) = variables;
		variables->prev = NULL;
	}
	else
	{
		last->next = variables;
		variables->prev = last;
	}
	variables->next = NULL;
	fill_variable(&variables, cmd, data);
}