/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:22:51 by juramos           #+#    #+#             */
/*   Updated: 2024/04/20 12:35:13 by juramos          ###   ########.fr       */
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

static char	*expand_word(char *str, int start, int end, char **envp)
{
	char	*keyword;
	char	*keyword_cleaned;
	char	*value;

	if (str[start + 1] == '?')
		return (ft_itoa(g_global.error_num));
	keyword = ft_substr(str, start + 1, end - start);
	keyword_cleaned = ft_strtrim(keyword, " ");
	value = ft_strdup(my_getenv(keyword_cleaned, envp));
	return (free(keyword_cleaned), value);
}

static char	*expand_str(char *str, int start, int *i, char **envp)
{
	char	*begin;
	char	*word;
	char	*end;
	char	*newstr;

	begin = ft_substr(str, 0, start);
	word = expand_word(str, start, *i, envp);
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
	return (free(newstr), free(end), begin);
}

char	*expand(char *str, int is_heredoc, char **envp)
{
	int		i;
	int		start;

	i = 0;
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'' && !is_heredoc)
		return (remove_quotes(str));
	if (str[0] == '\'' || str[0] == '\"')
		str = remove_quotes(str);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && !ft_isspace(str[i + 1]))
		{
			start = i;
			while (str[i] && !(ft_isspace(str[i])))
				i++;
			str = expand_str(str, start, &i, envp);
		}
		else
			i++;
	}
	return (str);
}
