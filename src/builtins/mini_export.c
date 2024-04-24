#include "minishell.h"

int	check_if_declaration(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

t_variable	*get_var_to_mod(t_variable *local_vars, int laps)
{
	t_variable	*it;
	int			i;

	it = local_vars;
	i = 0;
	while (i < laps)
	{
		it = it->next;
		i++;
	}
	return (it);
}

int		variable_in_env(t_variable *variable, char **env)
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

char	**modify_variable(char **env, t_minishell *data, t_variable *variable)
{
	int		nbr_env;
	int		i;
	int		name_size;
	int		value_size;
	char	**new_env;

	i = 0;
	name_size = ft_strlen(variable->name);
	value_size = ft_strlen(variable->value);
	nbr_env = get_nbr_env(env);
	new_env = (char **)malloc(sizeof(char *) * (nbr_env + 2));
	if (!new_env)
		error(data, "Memory problems in mini_export");
	while (i < nbr_env)
	{
		if (!ft_strncmp(env[i], variable->name, name_size) && ft_strncmp(env[i], variable->value, value_size))
		{
			new_env[i] = (char *)malloc(name_size + value_size + 2);
			if (!new_env[i])
				(free_arr(new_env), error(data, "Memory problems in mini_export"));
			ft_strlcpy(new_env[i], variable->name, name_size + 1);
			new_env[i][name_size] = '=';
			ft_strlcpy(new_env[i] + name_size + 1, variable->value, value_size + 1);
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
	int		i;
	char	**new_env;

	i = 0;
	nbr_env = get_nbr_env(env);
	new_env = (char **)malloc(sizeof(char *) * (nbr_env + 2));
	if (!new_env)
		error(data, "Memory problems in mini_export");
	while (i < nbr_env && env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			(free_arr(new_env), error(data, "Memory problems in mini_export"));
		i++;
	}
	new_env[i] = ft_strdup(variable);
	if (!new_env[i])
			(free_arr(new_env), error(data, "Memory problems in mini_export"));
	i++;
	new_env[i] = NULL;
	return (new_env);
}

char	*get_new_var(char *variable, t_variable *local_vars)
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
	//if (!new_var)
		//prot
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

// void swap(char **x, char **y) 
// {
//     char *temp;
	
// 	temp = *x;
//     *x = *y;
//     *y = temp;
// }

// int partition(char *arr[], int low, int high) 
// {
//     char	*pivot;
// 	int		i;
// 	int		j;
	
// 	pivor = arr[high]; 
//     i = (low - 1); 
// 	j = low;
// 	while (j <= (high - 1))
// 	{
// 		if (ft_strcmp(arr[j], pivor) < 0)
// 		{
// 			i++;
// 			swap(&arr[i], &arr[j]);
// 		}
// 		j++;
// 	}
//     swap(&arr[i + 1], &arr[high]);
//     return (i + 1);
// }

// void quicksort(char **arr, int low, int high) 
// {
// 	int pi;

//     if (low < high) 
// 	{
//         pi = partition(arr, low, high);
//         quicksort(arr, low, pi - 1);
//         quicksort(arr, pi + 1, high);
//     }
// }

// void	env_order(t_minishell *data)
// {
// 	char 	**sorted_env;
// 	int		n;
// 	int		i;

// 	sorted_env = ft_arrdup(data->env_vars);
// 	quicksort(sorted_env, 0, n - 1);
// 	while (sorted_env[i])
// 	{
// 		printf("declare -x %s")
// 	}


// }

void	mini_export(t_minishell *data)  // revisar orden en el que mete las variables en env en bash
{
	int		i;
	int		local_var;
	int		laps;
	char	**new_env;
	char	*new_var;

	i = 0;
	// if (data->n_args == 0)
	// 	env_order(data);
	while (i < data->cmd_table->n_args)
	{
		local_var = check_if_declaration(data->cmd_table->args[i]);
		laps = check_new_var(data->cmd_table->args[i], data->local_vars);
		if (!local_var)
		{
			if (laps < 0)
				create_new_variable(data->cmd_table->args[i], &(data->local_vars));
			else
				change_variable_value(data->cmd_table->args[i], &(data->local_vars), laps);
			if (laps > 0 && !variable_in_env(get_var_to_mod(data->local_vars, laps), data->env_vars))
				new_env = modify_variable(data->env_vars, data, get_var_to_mod(data->local_vars, laps));
			else
				new_env = add_variable(data->cmd_table->args[i], data->env_vars, data);
			free_arr(data->env_vars);
			data->env_vars = new_env;
		}
		else if (local_var && check_new_var(data->cmd_table->args[i], data->local_vars) >= 0 && variable_in_env(get_var_to_mod(data->local_vars, laps), data->env_vars))
		{
			new_var = get_new_var(data->cmd_table->args[i], data->local_vars);
			new_env = add_variable(new_var, data->env_vars, data);
			free_arr(data->env_vars);
			free(new_var);
			data->env_vars = new_env;
		}
		i++;
	}
}
