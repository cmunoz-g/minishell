/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:59:49 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/10 11:49:32 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	declaration(t_minishell *data, int i, int laps, char **new_env)
{
	char	**new_export;
	
	if (!variable_in_env_char(data->cmd_table->args[i], data->env_vars))
	{
		new_env = mod_var(data->env_vars, data, data->cmd_table->args[i]);
		new_export = mod_var(data->export_vars, data, data->cmd_table->args[i]);
	}
	else
	{
		if (laps < 0)
			create_new_variable(data->cmd_table->args[i],
				&(data->local_vars), data);
		else
			change_var_value(data->cmd_table->args[i],
				&(data->local_vars), laps, data);
		if (laps >= 0 && !variable_in_env(get_var_to_mod(data->local_vars,
					laps), data->env_vars))
			{
				new_env = mod_var(data->env_vars, data, data->cmd_table->args[i]);
				new_export = mod_var_export(data->export_vars, data, data->cmd_table->args[i]);
			}
		else
		{
			new_env = add_variable(data->cmd_table->args[i],
					data->env_vars, data);
			if (!variable_in_env_char(data->cmd_table->args[i], data->export_vars))
			{
				new_export = mod_var_export(data->export_vars, data, data->cmd_table->args[i]); 
			}
			else
				new_export = add_variable(data->cmd_table->args[i], data->export_vars, data);
		}
	}
	(free_arr(data->export_vars), free_arr(data->env_vars));
	data->env_vars = new_env;
	data->export_vars = new_export;
}

char	**add_var_no_value(char *variable, char **exp, t_minishell *data)
{
	int		nbr_export;
	int		i_j[2];
	char	**new_export;

	i_j[0] = 0;
	i_j[1] = 0;
	nbr_export = get_nbr_env(exp);
	new_export = (char **)malloc(sizeof(char *) * (nbr_export + 2));
	if (!new_export)
		error(data, "Memory problems in mini_export");
	while (i_j[0] < nbr_export && exp[i_j[0]])
	{
		new_export[i_j[1]] = ft_strdup(exp[i_j[0]]);
		if (!new_export[i_j[1]])
			(free_arr(new_export), error(data, "Memory problems in mini_export"));
		i_j[0]++;
		i_j[1]++;
	}
	new_export[i_j[1]] = ft_strdup(variable);
	if (!new_export[i_j[0]])
		(free_arr(new_export), error(data, "Memory problems in mini_export"));
	i_j[1]++;
	new_export[i_j[1]] = NULL;
	return (new_export);
}

void	no_declaration(t_minishell *data, int i, char *new_var, char **new_env)
{
	char	**new_export;
	
	if (check_new_var(data->cmd_table->args[i], data->local_vars) >= 0)
	{
		new_var = get_new_var(data->cmd_table->args[i], data->local_vars, data);
		new_env = add_variable(new_var, data->env_vars, data);
		new_export = add_variable(new_var, data->export_vars, data); 
		(free_arr(data->env_vars), free(new_var), free_arr(data->export_vars));
		data->env_vars = new_env;
		data->export_vars = new_export;
	}
	else
	{
		new_export = add_var_no_value(data->cmd_table->args[i], data->export_vars, data);
		free_arr(data->export_vars);
		data->export_vars = new_export;
	}
}

void	reset_export(t_minishell *data, int i, int *declaration, int *laps)
{
	*declaration = check_if_declaration(data->cmd_table->args[i]);
	*laps = check_new_var(data->cmd_table->args[i], data->local_vars);
}

int	mini_export(t_minishell *data)
{
	int		i;
	int		dec;
	int		laps;
	char	**new_env;
	char	*new_var;

	i = 0;
	if (data->cmd_table->n_args == 0)
		env_order(data);
	new_env = NULL;
	new_var = NULL;
	while (i < data->cmd_table->n_args)
	{
		reset_export(data, i, &dec, &laps);
		if (!dec)
			declaration(data, i, laps, new_env);
		if (dec)
			no_declaration(data, i, new_var, new_env);
		i++;
	}
	return (0);
}
