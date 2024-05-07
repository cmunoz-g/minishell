/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:15:16 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 11:17:30 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(t_token *token)
{
	if (!token)
		printf("minishell: syntax error near unexpected token 'newline'\n");
	else
		printf("minishell: syntax error near unexpected token '%s'\n",
			token->value);
	g_global.error_num = 2;
}

int	check_syntax_redir(t_token *it)
{
	if (!it->next)
	{
		print_syntax_error(it->next);
		return (1);
	}
	else if (it->next && !ft_strcmp(it->next->value, "&"))
	{
		if (it->type == APPEND || it->type == HEREDOC)
			print_syntax_error(it->next);
		else
			print_syntax_error(NULL);
		return (1);
	}
	else if (it->next->type != FILENAME)
	{
		print_syntax_error(it->next);
		return (1);
	}
	return (0);
}

int	check_syntax_pipe(t_token *it)
{
	if ((!it->prev && !it->next) || (!it->next))
	{
		print_syntax_error(it);
		return (1);
	}
	if (it->next && (it->next->type != CMD))
	{
		print_syntax_error(it);
		return (1);
	}
	return (0);
}

int	check_syntax_end(t_token *it)
{
	if (!it->prev && !it->next)
	{
		print_syntax_error(it);
		return (1);
	}
	if (it->next && (it->next->type == END || it->next->type == EMPTY))
	{
		print_syntax_error(it);
		return (1);
	}
	return (0);
}

int	check_syntax(t_token *token_list)
{
	t_token	*it;

	it = token_list;
	while (it)
	{
		if (it->type == TRUNC || it->type == INPUT
			|| it->type == APPEND || it->type == HEREDOC)
		{
			if (check_syntax_redir(it))
				return (1);
		}
		else if (it->type == PIPE || it->type == DOUBLEPIPE)
		{
			if (check_syntax_pipe(it))
				return (1);
		}
		else if (it->type == END)
		{
			if (check_syntax_end(it))
				return (1);
		}
		it = it->next;
	}
	return (0);
}
