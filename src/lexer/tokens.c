/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:26:41 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/08 12:33:48 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_last_token(t_token *token_list)
{
	if (!token_list)
		return (0);
	while (token_list->next)
		token_list = token_list->next;
	return (token_list);
}

void	add_token(t_token **token_list, char *cmd_line, int start, int end)
{
	t_token	*new_token;
	t_token	*last;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		clean_token_list(token_list);
	last = get_last_token(*token_list);
	if (!last)
	{
		*token_list = new_token;
		new_token->prev = NULL;
	}
	else
	{
		last->next = new_token;
		new_token->prev = last;
	}
	new_token->value = ft_strdup_mod(cmd_line + start, end - start);
	new_token->next = NULL;
	new_token->variable = NULL;
	get_token_type(new_token);
}

// return 1 si ha de ser ARG, return 0 si ha de ser CMD

// a="hola", para hola debe retornar 1
// a="hola" ls, para hola debe retornar 1, para ls 0
// a="hola" "adios", ha de retornar 1 para ambos
// a=1 ls, ha de retornar 1 para ls

void	get_token_type(t_token *token)
{
	if (!ft_strcmp(token->value, ""))
		token->type = EMPTY;
	else if (token->value[0] == '>' && token->value[1] == '>')
		token->type = APPEND;
	else if (token->value[0] == '<' && token->value[1] == '<')
		token->type = HEREDOC;
	else if (token->value[0] == '<')
		token->type = INPUT;
	else if (token->value[0] == '>')
		token->type = TRUNC;
	else if (!ft_strcmp(token->value, "|"))
		token->type = PIPE;
	else if (!ft_strcmp(token->value, "||"))
		token->type = DOUBLEPIPE;
	else if (!ft_strcmp(token->value, ";"))
		token->type = END;
	else if (token->prev && (token->prev->type == TRUNC
			|| token->prev->type == APPEND || token->prev->type == INPUT
			|| token->prev->type == HEREDOC))
		token->type = FILENAME;
	else if (token->prev == NULL || (token->prev->type >= 6))
		token->type = CMD;
	else
		token->type = ARG;
}
