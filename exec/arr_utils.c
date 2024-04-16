/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:59:52 by juramos           #+#    #+#             */
/*   Updated: 2024/04/16 10:29:39 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	get_str_arr_len(char **strarr)
{
	int	n;

	if (!strarr)
		return (0);
	n = 0;
	while (strarr[n])
		n++;
	return (n);
}

char	**ft_str_arr_join_exec(char *s1, char **strarr, char **envp)
{
	int		i;
	char	**str3;
	char	*expanded;
	int		len;

	len = get_str_arr_len(strarr);
	i = 0;
	str3 = ft_calloc(sizeof(char *), len + 2);
	str3[i++] = s1;
	if (!strarr)
		return (str3);
	while (i < len + 1)
	{
		expanded = expand(strarr[i - 1], 0, envp);
		if (!expanded || !ft_strlen(expanded))
			str3[i] = NULL;
		else
			str3[i] = expanded;
		i++;
	}
	return (str3);
}

void	free_split(char **arr)
{
	int	pos;

	pos = -1;
	while (arr[++pos])
		free(arr[pos]);
	free(arr);
}
