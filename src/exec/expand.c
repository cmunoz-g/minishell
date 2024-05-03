/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:22:51 by juramos           #+#    #+#             */
/*   Updated: 2024/05/03 11:40:57 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_quotes(char *str)
{
	char	*keyword_cleaned;
	char	*keyword;

	keyword_cleaned = ft_strtrim(str, "\"");
	keyword = ft_strtrim(keyword_cleaned, "\'");
	return (free(keyword_cleaned), keyword);
}

static char	*expand_local(char *str, t_variable *vars)
{
	if (!vars)
		return (NULL);
	get_first_variable(&vars);
	while (vars)
	{
		if (!ft_strncmp(str, vars->name, ft_strlen(vars->name)))
			return (vars->value);
		vars = vars->next;
	}
	return (NULL);
}

static char	*expand_word(char *str, int start, int end, t_minishell *data)
{
	char	*keyword;
	char	*keyword_cleaned;
	char	*value;

	if (str[start + 1] == '?')
		return (ft_itoa(g_global.error_num));
	keyword = ft_substr(str, start + 1, end - start);
	keyword_cleaned = ft_strtrim(keyword, " ");
	free(keyword);
	value = ft_strdup(my_getenv(keyword_cleaned, data->env_vars));
	if (!value)
		value = ft_strdup(expand_local(keyword_cleaned, data->local_vars));
	return (free(keyword_cleaned), value);
}

static char	*expand_str(char *str, int start, int *i, t_minishell *data)
{
	char	*begin;
	char	*word;
	char	*end;
	char	*newstr;

	begin = ft_substr(str, 0, start);
	word = expand_word(str, start, *i, data);
	end = ft_substr(str, *i, ft_strlen(str) - *i);
	if (!word)
	{
		*i = start - 1;
		newstr = ft_strdup(begin);
	}
	else
	{
		*i = start + ft_strlen(word) - 1;
		newstr = ft_strjoin(begin, word);
		free(word);
	}
	free(begin);
	begin = ft_strjoin(newstr, end);
	return (free(newstr), free(end), free(str), begin);
}

char	*expand(char *str, int is_heredoc, t_minishell *data)
{
	int		i;
	int		start;
	char	*ret;

	i = 0;
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'' && !is_heredoc)
		return (remove_quotes(str));
	else if (str[0] == '\'' || str[0] == '\"')
		ret = remove_quotes(str);
	else
		ret = ft_strdup(str);
	while (ret[i])
	{
		if (ret[i] == '$' && ret[i + 1] && !ft_isspace(ret[i + 1]))
		{
			start = i;
			while (ret[i] && !(ft_isspace(ret[i])))
				i++;
			ret = expand_str(ret, start, &i, data);
		}
		else
			i++;
	}
	return (ret);
}
