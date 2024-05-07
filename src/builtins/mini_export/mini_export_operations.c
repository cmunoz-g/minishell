/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_operations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:59:52 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 16:08:45 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
