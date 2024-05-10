/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:01:30 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/10 10:34:11 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_token_list(t_token **token_list)
{
	t_token	*tmp;

	while (*token_list)
	{
		tmp = (*token_list)->next;
		free((*token_list)->value);
		(*token_list)->value = NULL;
		free(*token_list);
		(*token_list) = tmp;
	}
	(*token_list) = NULL;
}

void	clean_data(t_minishell **data)
{
	if ((*data)->line)
		free((*data)->line);
	if ((*data)->env_vars)
		free_arr((*data)->env_vars);
	if ((*data)->export_vars)
		free_arr((*data)->export_vars);
	if ((*data)->pwd)
		free((*data)->pwd);
	if ((*data)->old_pwd)
		free((*data)->old_pwd);
	if ((*data)->local_vars)
		clean_local_vars(&(*data)->local_vars);
	if ((*data)->cmd_table)
		clean_cmd_table_list(&((*data)->cmd_table));
	rl_clear_history();
	free((*data));
}

void	get_first_variable(t_variable **var)
{
	while ((*var)->prev)
		*var = (*var)->prev;
}

void	clean_local_vars(t_variable **local_vars)
{
	t_variable	*it;

	get_first_variable(local_vars);
	while ((*local_vars))
	{
		it = (*local_vars)->next;
		if ((*local_vars)->name)
			free((*local_vars)->name);
		((*local_vars)->name) = NULL;
		if ((*local_vars)->value)
			free((*local_vars)->value);
		((*local_vars)->value) = NULL;
		free(*local_vars);
		(*local_vars) = it;
	}
	(*local_vars) = NULL;
}
