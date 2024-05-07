/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 12:16:35 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 13:14:19 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

void	change_var_value(char *cmd, t_variable **local_vars, int laps, t_minishell *data)
{
	t_variable	*it;
	int			i;
	int			value_size;
	int			name_size;

	value_size = get_var_size(cmd, false);
	name_size = get_var_size(cmd, true);
	it = *local_vars;
	i = 0;
	while (i < laps && it) 
	{
		it = it->next;
		i++;
	}
	free(it->value);
	it->value = (char *)malloc(sizeof(char) * (value_size + 1));
	if (!it->value)
		error(data, "Memory problems while creating a new variable");
	ft_strlcpy(it->value, cmd + name_size + 1, value_size + 1);
}

char	**change_var_value_env(t_variable *variable, char **env, t_minishell *data)
{
	char	**new_env;
	int		nbr_env;
	int		name_size;
	int		value_size;
	int		i;

	nbr_env = get_nbr_env(env);
	new_env = (char **)malloc(sizeof(char *) * (nbr_env + 1));
	if (!new_env)
		error(data, "Memory problems while modifying env");
	name_size = ft_strlen(variable->name);
	value_size = ft_strlen(variable->value);
	i = 0;
	while (i < nbr_env && env[i])
	{
		if (!ft_strncmp(variable->name, env[i], name_size))
		{
			new_env[i] = ft_strdup(variable->name);
			if (!new_env[i])
				error(data, "Memory problems while modifying env");
			new_env[i] = (char *)malloc(name_size + value_size + 2);
			if (!new_env[i])
				(free_arr(new_env), error(data, "Memory problems while modifying env"));
			ft_strlcpy(new_env[i], variable->name, name_size + 1);
			new_env[i][name_size] = '=';
			ft_strlcpy(new_env[i] + name_size + 1, variable->value, value_size + 1);
		}
		else
		{
			new_env[i] = ft_strdup(env[i]);
			if (!new_env[i])
				error(data, "Memory problems while modifying env");
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	local_variables_aux(t_minishell *data, t_cmd_table *tmp, char **new_env)
{
	int	laps;
	
	laps = check_new_var(tmp->cmd, data->local_vars);
	if (laps < 0)
		create_new_variable(tmp->cmd, &(data->local_vars), data);
	else
	{
		change_var_value(tmp->cmd, &(data->local_vars), laps, data); 
		if (!variable_in_env(get_var_to_mod(data->local_vars, laps), data->env_vars))
		{
			printf("hola");
			exit(0);
			new_env = change_var_value_env(get_var_to_mod(data->local_vars, laps), data->env_vars, data);
			free_arr(data->env_vars);
			data->env_vars = new_env;
		}
	}
}

void	local_variables(t_minishell *data)
{
	t_cmd_table	*tmp;
	char		**new_env;


	tmp = data->cmd_table;
	new_env = NULL;
	while (tmp)
	{
		if (!check_variable(tmp))
		{
			if (!variable_in_env_char(tmp->cmd, data->env_vars))
			{
				new_env = modify_variable(data->env_vars, data, tmp->cmd);
				free_arr(data->env_vars);
				data->env_vars = new_env;
			}
			else
				local_variables_aux(data, tmp, new_env);
			tmp = tmp->next; 
		}
		else
			tmp = tmp->next;
	}
}
