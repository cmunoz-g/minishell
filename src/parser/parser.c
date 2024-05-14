/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:26:15 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/13 12:35:27 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		parser_conditions(t_token *token)
{
	if (token->type == PIPE)
		return (0);
	else if (token->next && token->next->type == CMD)
	{
		if (!is_pos_variable(token->next->value))
			return (0);
		else if (!is_pos_variable(token->value))
			return (0);
		else
			return (1);
	}
	return (1);
}

void	parser(t_cmd_table **cmd_table, t_token **token_list) 
{
	t_token		*tmp;
	int			s_e[2];

	init_parser(s_e, &tmp, token_list);
	while (*token_list)
	{
		if (!parser_conditions(*token_list))
		{
			alloc_cmd_table(cmd_table);
			gen_cmd_table(tmp, cmd_table, s_e[0], s_e[1] + 1);
			s_e[0] = s_e[1];
		}
		parser_aux(token_list, s_e);
	}
	if (*cmd_table)
	{
		alloc_cmd_table(cmd_table);
		gen_cmd_table(tmp, cmd_table, s_e[0] + 1, s_e[1]);
	}
	else
	{
		alloc_cmd_table(cmd_table);
		gen_cmd_table(tmp, cmd_table, s_e[0], s_e[1]);
	}
}

void	alloc_cmd_table(t_cmd_table **cmd_list)
{
	t_cmd_table	*cmd_table;
	t_cmd_table	*last;

	cmd_table = (t_cmd_table *)malloc(sizeof(t_cmd_table));
	if (!cmd_table)
		clean_cmd_table_list(*cmd_list);
	last = get_last_cmd_table(*cmd_list);
	if (!(*cmd_list))
	{
		(*cmd_list) = cmd_table;
		cmd_table->prev = NULL;
	}
	else
	{
		last->next = cmd_table;
		cmd_table->prev = last;
	}
	init_cmd_table(&cmd_table);
}

void	init_parser(int start_end[2], t_token **tmp, t_token **token_list)
{
	start_end[0] = 0;
	start_end[1] = 0;
	(*tmp) = (*token_list);
}

void	parser_aux(t_token **token_list, int *s_e)
{
	s_e[1]++;
	(*token_list) = (*token_list)->next;
}
