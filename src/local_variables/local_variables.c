#include "minishell.h" 

// la idea es comprobar todos los cmd, si cumplen los requisitos se convierten en variables y se guardan en en local_vars dentro de t_minishell *data
// y se elimina el cmd de la lista

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

void	fill_variable(t_variable **variables, char *cmd)
{
	int	name_size;
	int	value_size;
	
	name_size = get_var_size(cmd, true);
	value_size = get_var_size(cmd, false);

	(*variables)->name = (char *)malloc(sizeof(char) * (name_size + 1));
	// if (!(*variables)->name)
	// 	//gestionar
	ft_strlcpy((*variables)->name, cmd, name_size + 1);
	(*variables)->value = (char *)malloc(sizeof(char) * (value_size + 1));
	// if (!(*variables)->value)
	// 	// gestionar
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

void	create_new_variable(char *cmd, t_variable **local_vars)
{
	t_variable *last;
	t_variable *variables;

	variables = (t_variable *)malloc(sizeof(t_variable));
	if (!variables)
		// gestionar error
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
	fill_variable(&variables, cmd);
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

void	take_cmd_out(t_cmd_table **cmd_table) // ARREGLAR
{
	// int	i;
	// int j;

	// i = 0;
	// j = 0;
	if ((*cmd_table)->prev)
		(*cmd_table)->prev->next = (*cmd_table)->next;
	if ((*cmd_table)->next)
		(*cmd_table)->next->prev = (*cmd_table)->prev;
	// if ((*cmd_table)->n_args)
	// {
	// 	while ((*cmd_table)->args[i])
	// 		free((*cmd_table)->args[i++]);
	// 	free((*cmd_table)->args);
	// }
	// if ((*cmd_table)->redirections)
	// 	clean_cmd_table_redir(cmd_table, &j);
	// if ((*cmd_table)->cmd)
	// 	free((*cmd_table)->cmd);
	// free(*cmd_table);
}

void	local_variables(t_minishell *data)
{
	t_cmd_table	*tmp;
	//t_cmd_table *next;

	tmp = data->cmd_table;
	while (tmp)
	{
		if (!check_variable(tmp))
		{
			create_new_variable(tmp->cmd, &(data->local_vars));
			take_cmd_out(&tmp); // creo que el problema es que esto al eliminar tmp, el tmp = tmp->next revienta. ver como gestionar el sacar un nodo de 
			// otra forma
		}
		tmp = tmp->next;
	}
}