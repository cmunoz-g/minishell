/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:17:45 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 11:23:46 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
