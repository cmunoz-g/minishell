/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comments_end.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:17:45 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/24 10:10:11 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_only_quote(t_token *token_list)
{
	t_token	*tmp;

	tmp = token_list;
	while (tmp)
	{
		if (ft_strncmp(token_list->value, "\'", 1)
			&& ft_strncmp(token_list->value, "\"", 1))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	take_out_tokens(t_token **tmp)
{
	t_token	*hold;
	t_token	*current;

	if (!tmp || !(*tmp))
		return ;
	current = (*tmp);
	if (current->prev)
		current->prev->next = NULL;
	while (current)
	{
		hold = current->next;
		free(current->value);
		current->value = NULL;
		free(current);
		current = hold;
	}
	(*tmp) = NULL;
}

int	check_comments(t_token **token_list)
{
	t_token	*tmp;
	int		first;

	tmp = (*token_list);
	first = 1;
	while (tmp)
	{
		if (tmp->value[0] == '#')
		{
			if (tmp == (*token_list))
				first = 0;
			take_out_tokens(&tmp);
			return (first);
		}
		tmp = tmp->next;
	}
	return (first);
}

void	check_end(t_token **token_list)
{
	t_token	*tmp;

	tmp = (*token_list);
	while (tmp)
	{
		if (tmp->type == END)
		{
			take_out_tokens(&tmp);
			return ;
		}
		tmp = tmp->next;
	}
}
