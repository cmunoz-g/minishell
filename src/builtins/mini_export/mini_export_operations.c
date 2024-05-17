/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_operations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:59:52 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/17 09:15:34 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**mod_var_export(char **export_vars, t_minishell *data, char *variable)
{
	int		nbr_export;
	int		i;
	int		size;
	char	**new_export;
	
	i = 0;
	size = get_var_size(variable, true);
	nbr_export = get_nbr_env(export_vars);
	new_export = (char **)malloc(sizeof(char *) * (nbr_export + 2));
	if (!new_export)
		error(data, "Memory problems while modifying export variables");
	while (export_vars[i])
	{
		if (!ft_strncmp(variable, export_vars[i], size))
		{
			new_export[i] = ft_strdup(variable); 
		}
		//proteget
		else
			new_export[i] = ft_strdup(export_vars[i]);
		// proteger
		i++;
	}
	new_export[i] = NULL;
	return (new_export);
}

void	mod_var_aux(t_minishell *data, char **new_env, int *n_v_i, char *var)
{
	if (!ft_strncmp(data->env_vars[n_v_i[2]], var,
			get_comp_size(n_v_i[0], data->env_vars[n_v_i[2]]))
		&& ft_strncmp(data->env_vars[n_v_i[2]], var + n_v_i[0], n_v_i[1]))
	{
		new_env[n_v_i[2]] = (char *)malloc(n_v_i[0] + n_v_i[1] + 2);
		if (!new_env[n_v_i[2]])
			(free_arr(new_env), error(data, "Memory problems in mini_export"));
		ft_strlcpy(new_env[n_v_i[2]], var, n_v_i[0] + 1);
		new_env[n_v_i[2]][n_v_i[0]] = '=';
		ft_strlcpy(new_env[n_v_i[2]] + n_v_i[0] + 1,
			var + n_v_i[0] + 1, n_v_i[1] + 1);
	}
	else
		new_env[n_v_i[2]] = ft_strdup(data->env_vars[n_v_i[2]]);
	if (!new_env[n_v_i[2]])
		(free_arr(new_env), error(data, "Memory problems in mini_export"));
}

// n, v, i: name, value, i.

char	**mod_var(char **env, t_minishell *data, char *variable)
{
	int		nbr_env;
	int		n_v_i[3];
	char	**new_env;

	n_v_i[0] = get_var_size(variable, true);
	n_v_i[1] = get_var_size(variable, false);
	n_v_i[2] = 0;
	if (n_v_i[1] == 0)
		n_v_i[1] = 1;
	nbr_env = get_nbr_env(env);
	new_env = (char **)malloc(sizeof(char *) * (nbr_env + 2));
	if (!new_env)
		error(data, "Memory problems in mini_export");
	while (n_v_i[2] < nbr_env)
	{
		mod_var_aux(data, new_env, n_v_i, variable);
		n_v_i[2]++;
	}
	new_env[n_v_i[2]] = NULL;
	return (new_env);
}

char	**add_variable(char *variable, char **env, t_minishell *data)
{
	int		nbr_env;
	int		i_j[2];
	char	**new_env;

	i_j[0] = 0;
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
