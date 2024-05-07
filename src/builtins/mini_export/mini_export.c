/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:59:49 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 16:01:43 by cmunoz-g         ###   ########.fr       */
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
	if (!variable_in_env_char(data->cmd_table->args[i], data->env_vars))
		new_env = mod_var(data->env_vars, data, data->cmd_table->args[i]);
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
			new_env = mod_var(data->env_vars, data, data->cmd_table->args[i]);
		else
			new_env = add_variable(data->cmd_table->args[i],
					data->env_vars, data);
	}
	free_arr(data->env_vars);
	data->env_vars = new_env;
}

void	no_declaration(t_minishell *data, int i, char *new_var, char **new_env)
{
	if (check_new_var(data->cmd_table->args[i], data->local_vars) >= 0)
	{
		new_var = get_new_var(data->cmd_table->args[i], data->local_vars, data);
		new_env = add_variable(new_var, data->env_vars, data);
		(free_arr(data->env_vars), free(new_var));
		data->env_vars = new_env;
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
