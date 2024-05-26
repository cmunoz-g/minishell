/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 12:16:35 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/26 10:53:18 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

void	change_var_value(char *cmd, t_variable **loc, int l, t_minishell *data)
{
	t_variable	*it;
	int			i;
	int			value_size;
	int			name_size;

	value_size = get_var_size(cmd, false);
	name_size = get_var_size(cmd, true);
	it = *loc;
	i = 0;
	while (i < l && it)
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

void	local_variables_aux(t_minishell *data, t_cmd_table *tmp)
{
	int	laps;

	laps = check_new_var(tmp->cmd, data->local_vars);
	if (laps < 0)
		create_new_variable(tmp->cmd, &(data->local_vars), data);
	else
		change_var_value(tmp->cmd, &(data->local_vars), laps, data);
}

void	add_local_variables(char **new_env,
	char **new_export, t_minishell *data, t_cmd_table *tmp)
{
	if (tmp->n_args)
		variable_with_quotes(&tmp, data);
	if (!variable_in_env_char(tmp->cmd, data->env_vars))
	{
		new_env = mod_var(data->env_vars, data, tmp->cmd);
		free_arr(data->env_vars);
		data->env_vars = new_env;
	}
	if (!variable_in_env_char(tmp->cmd, data->export_vars))
	{
		new_export = mod_var_export(data->export_vars, data, tmp->cmd);
		free_arr(data->export_vars);
		data->export_vars = new_export;
	}
	local_variables_aux(data, tmp);
}

int	break_variables_loop(t_minishell *data)
{
	clean_local_vars(&data->local_vars);
	cmd_table_no_vars(data);
	return (1);
}

void	local_variables(t_minishell *data)
{
	t_cmd_table	*tmp;
	char		**new_env;
	char		**new_export;
	char		*expanded;
	bool		var;

	tmp = data->cmd_table;
	new_env = NULL;
	new_export = NULL;
	var = false;
	while (tmp)
	{
		expanded = expand(tmp->cmd, 0, data);
		free(tmp->cmd);
		tmp->cmd = expanded;
		if (!check_variable(tmp))
		{
			add_local_variables(new_env, new_export, data, tmp);
			var = true;
		}
		else if (check_variable(tmp) && var == true)
			if (break_variables_loop(data))
				return ;
		tmp = tmp->next;
	}
}
