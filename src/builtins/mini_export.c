#include "minishell.h"

int	check_if_declaration(char *arg)
{
	int		i;
	bool	flag;

	i = 0;
	flag = false;
	while (arg[i])
	{
		if (arg[i] == '-')
			flag = true;
		if (!flag && arg[i] == '=' && i != 0)
			return (0);
		else if (arg[i] == '=' && i == 0)
		{
			printf("minishell: export: '%s': not a valid identifier\n", arg);
			g_global.error_num = 1;
		}
		i++;
	}
	if (flag)
		printf("minishell: export: '%s': not a valid identifier\n", arg);
	return (1);
}

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
		if (!ft_strncmp(env[i], variable, name_size) && ft_strncmp(env[i], variable + name_size, value_size))
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

void swap(char **x, char **y) 
{
    char *temp;
	
	temp = *x;
    *x = *y;
    *y = temp;
}

int partition(char *arr[], int low, int high) 
{
    char	*pivot;
	int		i;
	int		j;
	
	pivot = arr[high]; 
    i = (low - 1); 
	j = low;
	while (j <= (high - 1))
	{
		if (ft_strcmp(arr[j], pivot) < 0)
		{
			i++;
			swap(&arr[i], &arr[j]);
		}
		j++;
	}
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(char **arr, int low, int high) 
{
	int pi;

    if (low < high) 
	{
        pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

void	env_order(t_minishell *data)
{
	char 	**sorted_env;
	int		n;
	int		i;
	int		j;

	sorted_env = ft_arrdup(data->env_vars);
	n = get_nbr_env(sorted_env);
	quicksort(sorted_env, 0, n - 1);
	i = 0;
	j = 0;
	while (sorted_env[i])
	{
		printf("declare -x ");
		while (sorted_env[i][j] != '=')
			printf("%c", sorted_env[i][j++]);
		printf("=\"");
		j++;
		while (sorted_env[i][j])
			printf("%c", sorted_env[i][j++]);
		printf("\"\n");
		j = 0;
		i++;
	}
	free_arr(sorted_env);
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
	while (i < data->cmd_table->n_args)
	{
		declaration = check_if_declaration(data->cmd_table->args[i]);
		laps = check_new_var(data->cmd_table->args[i], data->local_vars);
		if (!declaration)
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

