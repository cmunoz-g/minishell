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

int		check_variable(t_cmd_table *cmd_table)
{
	int	i;
	
	i = 0;
	while (cmd_table->cmd[i])
	{
		if (cmd_table->cmd[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

int	check_new_var(char *cmd, t_variable *local_vars)
{
	int			equal_pos;
	t_variable	*it;
	int			i;

	equal_pos = get_var_size(cmd, true);
	it = local_vars;
	i = 0;
	while (it) 
	{
		if (!ft_strncmp(cmd, it->name, equal_pos))
			return (i);
		it = it->next;
		i++;
	}
	return (-1);
}

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

void	local_variables(t_minishell *data)
{
	t_cmd_table	*tmp;
	char		**new_env;
	int			laps;

	tmp = data->cmd_table;
	while (tmp)
	{
		if (!check_variable(tmp))
		{
			laps = check_new_var(tmp->cmd, data->local_vars);
			if (laps < 0)
				create_new_variable(tmp->cmd, &(data->local_vars), data);
			else
			{
				change_var_value(tmp->cmd, &(data->local_vars), laps, data); 
				if (!variable_in_env(get_var_to_mod(data->local_vars, laps), data->env_vars))
				{
					new_env = change_var_value_env(get_var_to_mod(data->local_vars, laps), data->env_vars, data);
					free_arr(data->env_vars);
					data->env_vars = new_env;
				}
			}
			tmp = tmp->next; 
		}
		else
			tmp = tmp->next;
	}
}