/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:41:01 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/16 10:08:33 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_qt(bool *quotes, char *quote_type, char *cmd_line, int i)
{
	(*quotes) = true;
	(*quote_type) = cmd_line[i];
}

void	init_lexer(bool *qt, int *i, int *start)
{
	*qt = false;
	*i = -1;
	*start = 0;
}
int		is_pos_variable(char *value)
{
	int	i;

	i = 0;
	if (value[i++] == '=')
		return (1);
	while (value[i])
	{
		if (value[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

int	is_quoted_var(char *value)
{
	int		i;
	char	quote_type;
	bool	is_quote;
	
	i = 0;
	is_quote = false;
	quote_type = '\0';
	while (value[i])
	{
		if (value[i] == quote_type)
			is_quote = false;
		if (value[i] == '\'' || value[i] == '\"')
		{
			is_quote = true;
			quote_type = value[i];
		}
		i++;
	}
	if (is_quote == false && quote_type == '\0')
		return (1);
	else 
		return (0);
}
