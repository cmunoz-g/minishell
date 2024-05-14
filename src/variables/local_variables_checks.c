/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables_checks.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 12:18:00 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/09 10:25:13 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_variable(t_cmd_table *cmd_table)
{
	int	i;

	i = 0;
	if (!cmd_table->cmd)
		return (1);
	if (cmd_table->cmd[i++] == '=')
		return (1);
	while (cmd_table->cmd[i])
	{
		if (cmd_table->cmd[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

int	check_new_var(char *cmd, t_variable *local_vars)
{
	int			equal_pos;
	t_variable	*it;
	int			i;

	equal_pos = get_var_size(cmd, true);
	it = local_vars;
	i = 0;
	while (it)
	{
		if (!ft_strncmp(cmd, it->name, equal_pos))
			return (i);
		it = it->next;
		i++;
	}
	return (-1);
}
