/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:22:51 by juramos           #+#    #+#             */
/*   Updated: 2024/05/16 10:18:10 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_local(char *str, t_variable *vars);
static char	*expand_str(char *str, int start, int *i, t_minishell *data);
static char	*expand_word(char *str, int start, int end, t_minishell *data);
static char	*remove_quotes(char *str);

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
		if (!ft_strncmp(str, vars->name, ft_strlen(vars->name))
			&& !ft_strncmp(str, vars->name, ft_strlen(str)))
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
	keyword = ft_strtrim(keyword_cleaned, "=");
	free(keyword_cleaned);
	value = ft_strdup(my_getenv(keyword, data->env_vars));
	if (!value)
		value = ft_strdup(expand_local(keyword, data->local_vars));
	return (free(keyword), value);
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
	if (ft_strlen(newstr) || ft_strlen(end))
		begin = ft_strjoin(newstr, end);
	else
		begin = NULL;
	return (free(newstr), free(end), free(str), begin);
}

char	*expand(char *str, int is_heredoc, t_minishell *data)
{
	int		i;
	int		start;
	char	*ret;

	i = 0;
	if (!str)
		return (NULL);
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'' && !is_heredoc)
		return (remove_quotes(str));
	ret = remove_quotes(str);
	while (ret[i])
	{
		if (ret[i] == '$' && ret[i + 1] && pass_over_expand(ret[i + 1]))
		{
			start = i;
			while (ret[i] && pass_over_expand(ret[i]))
				i++;
			ret = expand_str(ret, start, &i, data);
			if (!ret || !ft_strlen(ret) || i == -1)
				return (ret);
		}
		else
			i++;
	}
	return (ret);
}
