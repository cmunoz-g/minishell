/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:31:28 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/09 10:44:14 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	gen_cmd_table_redir(t_cmd_table *last, t_minishell *data)
{
	int	j;

	j = 0;
	if (last->n_redirections)
	{
		last->redirections = (t_token **)malloc(sizeof(t_token *)
				* last->n_redirections);
		if (!last->redirections)
			error(data, "Memory problems while creating redirections");
	}
	else
		last->redirections = NULL;
	while (j < last->n_redirections)
	{
		last->redirections[j] = (t_token *)malloc(sizeof(t_token));
		if (!last->redirections[j])
			error(data, "Memory problems while creating redirections");
		j++;
	}
}

void	gen_cmd_table(t_token *list, t_cmd_table **table, int start, int end)
{
	t_cmd_table	*last;
	int			i;

	last = get_last_cmd_table((*table));
	i = 0;
	while (i < start)
	{
		list = list->next;
		i++;
	}
	last->args = (char **)malloc(sizeof(char *)
			* (get_nbr_args(list, (end - start)) + 1));
	if (!last->args)
		error(list->data, "Memory problems while creating cmd arguments");
	last->n_redirections = get_nbr_redir(list, (end - start));
	gen_cmd_table_redir(last, list->data);
	populate_cmd_table(list, &last, (end - start));
}

void	init_cmd_table(t_cmd_table **cmd_table)
{
	(*cmd_table)->cmd = NULL;
	(*cmd_table)->args = NULL;
	(*cmd_table)->n_args = 0;
	(*cmd_table)->in = 0;
	(*cmd_table)->out = 0;
	(*cmd_table)->err = 0;
	(*cmd_table)->hd_file = NULL;
	(*cmd_table)->next = NULL;
	(*cmd_table)->n_redirections = 0;
	(*cmd_table)->redirections = NULL;
}

void	populate_cmd_table(t_token *list, t_cmd_table **cmd_table, int nbr_tok)
{
	int		i_j[2];
	int		w;
	char	redir;

	init_pop_cmd_table(i_j, &w, &redir, cmd_table);
	while (i_j[0] < nbr_tok)
	{
		if (list->type == CMD)
			(*cmd_table)->cmd = ft_strdup(list->value);
		else if (list->type == ARG)
			(*cmd_table)->args[i_j[1]++] = ft_strdup(list->value);
		else if (list->type == PIPE)
			(*cmd_table)->out = PIPE;
		else if ((list->type == STDOUT || list->type == STDERR)
			&& (w < (*cmd_table)->n_redirections))
			check_std_cmd_table(list, cmd_table, w);
		check_redir_cmd_table(list, &redir);
		if (list->type == FILENAME)
			assign_redir_cmd_t(list, cmd_table, &w, redir);
		i_j[0]++;
		list = list->next;
	}
	if ((*cmd_table)->args)
		(*cmd_table)->args[i_j[1]] = NULL;
	(*cmd_table)->n_args = i_j[1];
}

void	init_pop_cmd_table(int *i_j, int *w, char *redir, t_cmd_table **table)
{
	i_j[0] = 0;
	i_j[1] = 0;
	*w = 0;
	*redir = '\0';
	if ((*table)->prev)
		(*table)->in = PIPE;
	else
		(*table)->in = STDIN;
	(*table)->out = STDOUT;
	(*table)->err = STDERR;
}
