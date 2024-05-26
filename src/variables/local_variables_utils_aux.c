/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables_utils_aux.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:57:36 by camunozg          #+#    #+#             */
/*   Updated: 2024/05/24 10:02:32 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_get_new_var_space(int *res, int *i_j, t_quotes *quote)
{
	i_j[0] = 0;
	i_j[1] = 0;
	*res = 0;
	quote->qtt = '\0';
	quote->qt = false;
}

void	clean_table_var(t_cmd_table **tmp,
	t_cmd_table **next, t_minishell *data)
{
	int			i;

	i = 0;
	free((*tmp)->cmd);
	free_arr((*tmp)->args);
	while (i < (*tmp)->n_redirections)
	{
		free((*tmp)->redirections[i]->value);
		free((*tmp)->redirections[i++]);
	}
	free((*tmp)->redirections);
	if ((*tmp)->prev)
	{
		(*tmp)->prev->next = (*next)->next;
		(*tmp)->prev->in = STDIN;
	}
	else
	{
		(*next)->prev = NULL;
		data->cmd_table = (*next);
		(*next)->in = STDIN;
	}
	free((*tmp));
}

void	cmd_table_no_vars(t_minishell *data)
{
	t_cmd_table	*tmp;
	t_cmd_table	*next;

	tmp = data->cmd_table;
	while (tmp)
	{
		next = tmp->next;
		if (!check_variable(tmp))
			clean_table_var(&tmp, &next, data);
		tmp = next;
	}
}
