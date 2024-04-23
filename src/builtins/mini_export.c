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

void	mini_export(t_minishell *data) // MIRAR REDECLARACION CUANDO HAGO EXPORT a=1 y a ya tiene un valor previo
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
		if (!local_var)
		{
			laps = check_new_var(data->cmd_table->args[i], data->local_vars);
			if (laps < 0)
				create_new_variable(data->cmd_table->args[i], &(data->local_vars));
			else
				change_variable_value(data->cmd_table->args[i], &(data->local_vars), laps);
			new_env = add_variable(data->cmd_table->args[i], data->env_vars, data);
			free_arr(data->env_vars);
			data->env_vars = new_env;
		}
		else if (local_var && check_new_var(data->cmd_table->args[i], data->local_vars) >= 0)
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
