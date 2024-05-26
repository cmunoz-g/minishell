/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_utils_aux.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 12:31:01 by camunozg          #+#    #+#             */
/*   Updated: 2024/05/26 12:31:33 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	no_declaration_aux(t_minishell *data, int i, char **new_export)
{
	if (variable_in_env_char(data->cmd_table->args[i], data->export_vars))
	{
		new_export = add_var_no_value(data->cmd_table->args[i],
				data->export_vars, data);
		free_arr(data->export_vars);
		data->export_vars = new_export;
	}
}

int	check_all_numbers(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}

void	check_if_declaration_aux(char *arg, bool flag)
{
	if (!check_all_numbers(arg))
	{
		printf("minishell: export: '%s': not a valid identifier\n", arg);
		g_global.error_num = 1;
	}
	else if (flag)
	{
		printf("minishell: export: '%s': not a valid identifier\n", arg);
		g_global.error_num = 1;
	}
}

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
	check_if_declaration_aux(arg, flag);
	return (1);
}
