/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:30:00 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 11:30:49 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table	*get_last_cmd_table(t_cmd_table *cmd_list)
{
	if (!cmd_list)
		return (0);
	while (cmd_list->next)
		cmd_list = cmd_list->next;
	return (cmd_list);
}

int	get_nbr_args(t_token *token_list, int nbr_tokens)
{
	int		i;
	int		nbr_args;
	t_token	*it;

	i = 0;
	nbr_args = 0;
	it = token_list;
	while (i < nbr_tokens)
	{
		if (it->type == ARG)
			nbr_args++;
		i++;
		it = it->next;
	}
	return (nbr_args);
}

int	get_nbr_redir(t_token *token_list, int nbr_tokens)
{
	int		i;
	int		nbr_redir;
	t_token	*it;

	i = 0;
	nbr_redir = 0;
	it = token_list;
	while (i < nbr_tokens)
	{
		if (it->type == TRUNC || it->type == APPEND
			|| it->type == INPUT || it->type == HEREDOC)
			nbr_redir++;
		i++;
		it = it->next;
	}
	return (nbr_redir);
}
