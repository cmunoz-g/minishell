/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:25:09 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 12:13:53 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* EN LA FT DE ABAJO: if (token_list->next) // hacer pruebas en las que esto
no se cumple, a ver que pasa con la tabla */

void	check_std_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int w)
{
	if (token_list->type == STDERR)
	{
		if (token_list->next)
			(*cmd_table)->err = token_list->next->type;
	}
	else if (token_list->type == STDOUT)
	{
		if (token_list->next)
			(*cmd_table)->out = token_list->next->type;
	}
	if (token_list->next)
	{
		(*cmd_table)->redirections[w]->type = token_list->next->type;
		if (token_list->next->next)
			(*cmd_table)->redirections[w]->value
				= ft_strdup(token_list->next->next->value);
		else
			(*cmd_table)->redirections[w]->value = NULL;
	}
	else
		(*cmd_table)->redirections[w]->type = 0;
}

void	check_redir_cmd_table_aux(t_token *token_list, char *redir)
{
	if (token_list->type == INPUT)
	{
		if (!token_list->prev)
			*redir = 'i';
		else if (token_list->prev && (token_list->prev->type != STDERR
				&& token_list->prev->type != STDOUT))
			*redir = 'i';
	}
	if (token_list->type == HEREDOC)
	{
		if (!token_list->prev)
			*redir = 'h';
		else if (token_list->prev && (token_list->prev->type != STDERR
				&& token_list->prev->type != STDOUT))
			*redir = 'h';
	}
}

void	check_redir_cmd_table(t_token *token_list, char *redir)
{
	if (token_list->type == TRUNC)
	{
		if (!token_list->prev)
			*redir = 't';
		else if (token_list->prev && (token_list->prev->type != STDERR
				&& token_list->prev->type != STDOUT))
			*redir = 't';
	}
	else if (token_list->type == APPEND)
	{
		if (!token_list->prev)
			*redir = 'a';
		else if (token_list->prev && (token_list->prev->type != STDERR
				&& token_list->prev->type != STDOUT))
			*redir = 'a';
	}
	check_redir_cmd_table_aux(token_list, redir);
}

void	assign_redir_cmd_t(t_token *list, t_cmd_table **table, int *w, char r)
{
	if (r == 'i' && *w < (*table)->n_redirections)
	{
		(*table)->in = INPUT;
		assign_redir_cmd_table_aux(table, w, INPUT, list->value);
	}
	else if (r == 'h' && *w < (*table)->n_redirections)
	{
		(*table)->in = HEREDOC;
		assign_redir_cmd_table_aux(table, w, HEREDOC, list->value);
	}
	else if (r == 't' && *w < (*table)->n_redirections)
	{
		(*table)->out = TRUNC;
		assign_redir_cmd_table_aux(table, w, TRUNC, list->value);
	}
	else if (r == 'a' && *w < (*table)->n_redirections)
	{
		(*table)->out = APPEND;
		assign_redir_cmd_table_aux(table, w, APPEND, list->value);
	}
}

void	assign_redir_cmd_table_aux(t_cmd_table **table, int *w, int t, char *v)
{
	(*table)->redirections[*w]->type = t;
	(*table)->redirections[*w]->value = ft_strdup(v);
	(*w)++;
}
