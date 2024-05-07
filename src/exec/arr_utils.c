/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:59:52 by juramos           #+#    #+#             */
/*   Updated: 2024/05/07 13:23:46 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	**ft_str_arr_join_exec(char *s1, char **strarr, t_minishell *data)
{
	int		i;
	char	**str3;
	char	*expanded;
	int		len;

	len = get_str_arr_len(strarr);
	i = 0;
	str3 = ft_calloc(sizeof(char *), len + 2);
	str3[i++] = expand(s1, 0, data);
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
