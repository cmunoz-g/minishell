#include "minishell.h"

char	**modify_variable(char **env, t_minishell *data, char *variable)
{
	int		nbr_env;
	int		i;
	int		name_size;
	int		value_size;
	char	**new_env;

	i = 0;
	name_size = get_var_size(variable, true);
	value_size = get_var_size(variable, false);
	if (value_size == 0)
		value_size = 1;
	nbr_env = get_nbr_env(env);
	new_env = (char **)malloc(sizeof(char *) * (nbr_env + 2));
	if (!new_env)
		error(data, "Memory problems in mini_export");
	while (i < nbr_env)
	{
		if (!ft_strncmp(env[i], variable, get_var_size(env[i], true)) && ft_strncmp(env[i], variable + name_size, value_size))
		{
			new_env[i] = (char *)malloc(name_size + value_size + 2);
			if (!new_env[i])
				(free_arr(new_env), error(data, "Memory problems in mini_export"));
			ft_strlcpy(new_env[i], variable, name_size + 1);
			new_env[i][name_size] = '=';
			ft_strlcpy(new_env[i] + name_size + 1, variable + name_size + 1, value_size + 1);
		}
		else
			new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			(free_arr(new_env), error(data, "Memory problems in mini_export"));
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	**add_variable(char *variable, char **env, t_minishell *data)
{
	int		nbr_env;
	int		i_j[2];
	char	**new_env;

	i_j[0]= 0;
	i_j[1] = 0;
	nbr_env = get_nbr_env(env);
	new_env = (char **)malloc(sizeof(char *) * (nbr_env + 2));
	if (!new_env)
		error(data, "Memory problems in mini_export");
	while (i_j[0] < nbr_env && env[i_j[0]])
	{
		new_env[i_j[1]] = ft_strdup(env[i_j[0]]);
		if (!new_env[i_j[1]])
			(free_arr(new_env), error(data, "Memory problems in mini_export"));
		i_j[0]++;
		i_j[1]++;
	}
	new_env[i_j[1]] = ft_strdup(variable);
	if (!new_env[i_j[0]])
		(free_arr(new_env), error(data, "Memory problems in mini_export"));
	i_j[1]++;
	new_env[i_j[1]] = NULL;
	return (new_env);
}

char	*get_new_var(char *variable, t_variable *local_vars, t_minishell *data)
{
	t_variable	*it;
	int			name_size;
	char		*new_var;
	int			i;
	int			j;

	it = local_vars;
	name_size = get_var_size(variable, true);
	while (ft_strncmp(it->name, variable, name_size))
		it = it->next;
	new_var = (char *)malloc(ft_strlen(it->name) + ft_strlen(it->value) + 2);
	if (!new_var)
		error(data, "Memory problems while creating a new variable");
	i = 0;
	j = 0;
	while (it->name[i])
		new_var[j++] = it->name[i++];
	i = 0;
	new_var[j++] = '=';
	while (it->value[i])
		new_var[j++] = it->value[i++];
	new_var[j] = '\0';
	return (new_var);
}

void	no_declaration(t_minishell *data, int i, int laps, char **new_env)
{
	if (!variable_in_env_char(data->cmd_table->args[i], data->env_vars))
			new_env = modify_variable(data->env_vars, data, data->cmd_table->args[i]);
	else
	{
		if (laps < 0)
			create_new_variable(data->cmd_table->args[i], &(data->local_vars), data);
		else
			change_var_value(data->cmd_table->args[i], &(data->local_vars), laps, data);
		if (laps >= 0 && !variable_in_env(get_var_to_mod(data->local_vars, laps), data->env_vars))
			new_env = modify_variable(data->env_vars, data, data->cmd_table->args[i]);
		else
			new_env = add_variable(data->cmd_table->args[i], data->env_vars, data);
	}
	free_arr(data->env_vars);
		data->env_vars = new_env;
}

int	mini_export(t_minishell *data) 
{
	int		i;
	int		declaration;
	int		laps;
	char	**new_env;
	char	*new_var;

	i = 0;
	if (data->cmd_table->n_args == 0)
		env_order(data);
	new_env = NULL;
	while (i < data->cmd_table->n_args)
	{
		declaration = check_if_declaration(data->cmd_table->args[i]);
		laps = check_new_var(data->cmd_table->args[i], data->local_vars);
		if (!declaration)
			no_declaration(data, i, laps, new_env);
		else if (declaration && check_new_var(data->cmd_table->args[i], data->local_vars) >= 0 && variable_in_env(get_var_to_mod(data->local_vars, laps), data->env_vars))
		{
			new_var = get_new_var(data->cmd_table->args[i], data->local_vars, data);
			new_env = add_variable(new_var, data->env_vars, data);
			free_arr(data->env_vars);
			free(new_var);
			data->env_vars = new_env;
		}
		i++;
	}
	return (0);
}

