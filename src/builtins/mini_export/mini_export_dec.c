/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_dec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 12:30:06 by camunozg          #+#    #+#             */
/*   Updated: 2024/05/26 12:31:38 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	declaration_aux(t_minishell *data, int laps, int i)
{
	if (laps < 0)
		create_new_variable(data->cmd_table->args[i],
			&(data->local_vars), data);
	else
		change_var_value(data->cmd_table->args[i],
			&(data->local_vars), laps, data);
}

void	declaration_end(t_minishell *data, char **new_env, char **new_export)
{
	(free_arr(data->export_vars), free_arr(data->env_vars));
	data->env_vars = new_env;
	data->export_vars = new_export;
}

void	declaration_new(t_minishell *data, int i,
	char ***new_export, char ***new_env)
{
	char	**tmp;
	char	**tmp_aux;

	*new_env = mod_var(data->env_vars, data, data->cmd_table->args[i]);
	*new_export = mod_var(data->export_vars, data, data->cmd_table->args[i]);
	tmp = *new_env;
	tmp++;
	tmp_aux = *new_export;
	tmp_aux++;
}

void	declaration_new_aux(t_minishell *data, int i,
	char ***new_export, char ***new_env)
{
	char	**tmp;
	char	**tmp_aux;

	*new_env = mod_var(data->env_vars, data, data->cmd_table->args[i]);
	*new_export = mod_var_export(data->export_vars,
			data, data->cmd_table->args[i]);
	tmp = *new_env;
	tmp++;
	tmp_aux = *new_export;
	tmp_aux++;
}

void	declaration(t_minishell *data, int i, int laps, char **new_env)
{
	char	**new_export;

	new_export = NULL;
	if (!variable_in_env_char(data->cmd_table->args[i], data->env_vars))
		declaration_new(data, i, &new_export, &new_env);
	else
	{
		declaration_aux(data, laps, i);
		if (laps >= 0 && !variable_in_env(get_var_to_mod(data->local_vars,
					laps), data->env_vars))
			declaration_new_aux(data, i, &new_export, &new_env);
		else
		{
			new_env = add_variable(data->cmd_table->args[i],
					data->env_vars, data);
			if (!variable_in_env_char(data->cmd_table->args[i],
					data->export_vars))
				new_export = mod_var_export(data->export_vars, data,
						data->cmd_table->args[i]);
			else
				new_export = add_variable(data->cmd_table->args[i],
						data->export_vars, data);
		}
	}
	declaration_end(data, new_env, new_export);
}
