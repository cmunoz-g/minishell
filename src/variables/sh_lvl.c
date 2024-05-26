/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_lvl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:54:36 by camunozg          #+#    #+#             */
/*   Updated: 2024/05/24 10:04:00 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_nbr_digits(int sh_lvl)
{
	int	res;

	res = 0;
	while (sh_lvl > 0)
	{
		sh_lvl /= 10;
		res++;
	}
	return (res);
}

void	modify_sh_lvl_export(char *tmp, t_minishell *data)
{
	char		**new_var_arr;
	static char	*str = "SHLVL=1";

	if (variable_in_env_char(tmp, data->export_vars))
	{
		new_var_arr = add_variable(str, data->export_vars, data);
		free_arr(data->export_vars);
		data->export_vars = new_var_arr;
	}
	else
	{
		new_var_arr = mod_var(data->export_vars, data, tmp);
		free_arr(data->export_vars);
		data->export_vars = new_var_arr;
	}
}

void	modify_sh_lvl(char *tmp, t_minishell *data)
{
	char		**new_var_arr;
	static char	*str = "SHLVL=1";

	if (variable_in_env_char(tmp, data->env_vars))
	{
		new_var_arr = add_variable(str, data->env_vars, data);
		free_arr(data->env_vars);
		data->env_vars = new_var_arr;
	}
	else
	{
		new_var_arr = mod_var(data->env_vars, data, tmp);
		free_arr(data->env_vars);
		data->env_vars = new_var_arr;
	}
	modify_sh_lvl_export(tmp, data);
}

void	update_sh_lvl(t_minishell *data, int operation)
{
	char	*tmp;
	char	*number_to_copy;

	data->sh_lvl += operation;
	tmp = (char *)malloc(6 + get_nbr_digits(data->sh_lvl) + 1);
	if (!tmp)
		error(data, "Could not update SHLVL");
	ft_strlcpy(tmp, "SHLVL=", 7);
	number_to_copy = ft_itoa(data->sh_lvl);
	if (!number_to_copy)
		error(data, "Could not update SHLVL");
	ft_strlcat(tmp, number_to_copy, ft_strlen(tmp)
		+ ft_strlen(number_to_copy) + 1);
	modify_sh_lvl(tmp, data);
	free(tmp);
	free(number_to_copy);
}
