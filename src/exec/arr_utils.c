/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:59:52 by juramos           #+#    #+#             */
/*   Updated: 2024/05/08 18:06:36 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_str_arr_len(char **strarr);

static int	get_str_arr_len(char **strarr)
{
	int	n;

	if (!strarr)
		return (EXIT_SUCCESS);
	n = 0;
	while (strarr[n])
		n++;
	return (n);
}

static char	**expand_cmd(char *s1, char **strarr, t_minishell *data, int *i)
{
	int		len;
	int		len_expanded;
	char	**str3;
	char	*expanded;
	char	**splitted_expanded;

	len = get_str_arr_len(strarr);
	expanded = expand(s1, 0, data);
	splitted_expanded = ft_split(expanded, ' ');
	free(expanded);
	len_expanded = get_str_arr_len(splitted_expanded);
	str3 = ft_calloc(sizeof(char *), len + len_expanded + 1);
	while (*i < len_expanded)
	{
		str3[*i] = splitted_expanded[*i];
		(*i)++;
	}
	return (str3);
}

char	**ft_str_arr_join_exec(char *s1, char **strarr, t_minishell *data)
{
	int		i;
	char	**str3;
	char	*expanded;
	int		len;

	len = get_str_arr_len(strarr);
	i = 0;
	str3 = expand_cmd(s1, strarr, data, &i);
	if (!strarr)
		return (str3);
	while (i < len + 1)
	{
		expanded = expand(strarr[i - 1], 0, data);
		if (!expanded || !ft_strlen(expanded))
			str3[i] = NULL;
		else
			str3[i] = expanded;
		i++;
	}
	return (str3);
}

void	free_arr(char **arr)
{
	int	pos;

	pos = -1;
	while (arr[++pos])
		free(arr[pos]);
	free(arr);
}

char	**ft_arrdup(char **arr)
{
	char	**ret;
	int		i;

	i = 0;
	while (arr[i])
		i++;
	ret = ft_calloc(sizeof(char *), i + 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		ret[i] = ft_strdup(arr[i]);
		if (!ret[i])
		{
			free_arr(ret);
			return (NULL);
		}
		i++;
	}
	return (ret);
}
