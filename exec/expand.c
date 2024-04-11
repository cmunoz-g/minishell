/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:22:51 by juramos           #+#    #+#             */
/*   Updated: 2024/04/11 13:12:57 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static char	*get_env_var(char *str)
{
	int		i;
	int		start;
	int		end;

	i = 0;
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'' && !is_heredoc)
		return (str);
	if (str[0] == '\'' || str[0] == '\"')
		i++;
	while (i < len)
	{
		if (str[i] == '$' && str[i + 1] && !ft_isspace(str[i + 1]))
		{
			start = i;
			i++;
			while (str[i] && !ft_isspace(str[i]))
				i++;
			end = i;
		}
	}
}
/*	expand:
	1. count number of $something in string
	2. create char **env_vars of size output of 1.
	3. when $something found, get variable equivalent and store it at env_vars.
	4. when all $somethings stored, replace their values at original string on new_str
	5. free env_vars, return new_str
*/
char	*expand(char *str, int is_heredoc)
{
	char	*expanded;
	int		i;

	i = 0;
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'' && !is_heredoc)
		return (str);
	if (str[0] == '\'' || str[0] == '\"')
		i++;
	while (i < len)
	{
		if (str[i] == '$' && str[i + 1] && !ft_isspace(str[i + 1]))

	}
}
