/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:08:40 by camunozg          #+#    #+#             */
/*   Updated: 2024/05/24 10:08:41 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_cmd_table_redir(t_cmd_table *cmd_table, int *j)
{
	if (!cmd_table)
		return ;
	while (*j < cmd_table->n_redirections)
	{
		free(cmd_table->redirections[*j]->value);
		cmd_table->redirections[*j]->value = NULL;
		free(cmd_table->redirections[*j]);
		cmd_table->redirections[*j] = NULL;
		(*j)++;
	}
	free(cmd_table->redirections);
	cmd_table->redirections = NULL;
}

t_cmd_table	*get_first_node(t_cmd_table *cmd)
{
	if (!cmd)
		return (NULL);
	while (cmd->prev)
		cmd = cmd->prev;
	return (cmd);
}

static void	clean_cmd_table_list_aux(t_cmd_table *cmd_table, int *j)
{
	if (cmd_table->n_redirections)
		clean_cmd_table_redir(cmd_table, j);
	if (cmd_table->cmd)
	{
		free(cmd_table->cmd);
		cmd_table->cmd = NULL;
	}
	if (cmd_table->hd_file)
	{
		free(cmd_table->hd_file);
		cmd_table->hd_file = NULL;
	}
}

void	clean_cmd_table_list(t_cmd_table *cmd_table)
{
	t_cmd_table	*tmp;
	int			i;
	int			j;

	i = 0;
	j = 0;
	cmd_table = get_first_node(cmd_table);
	while (cmd_table)
	{
		tmp = cmd_table->next;
		if (cmd_table->args)
		{
			while (cmd_table->args[i])
				free(cmd_table->args[i++]);
			free(cmd_table->args);
			cmd_table->args = NULL;
		}
		clean_cmd_table_list_aux(cmd_table, &j);
		free(cmd_table);
		i = 0;
		j = 0;
		cmd_table = tmp;
	}
	cmd_table = NULL;
}
