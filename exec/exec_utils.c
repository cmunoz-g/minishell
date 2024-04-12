/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:59:52 by juramos           #+#    #+#             */
/*   Updated: 2024/04/12 11:26:33 by juramos          ###   ########.fr       */
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
		printf("eyo: %s\n", expanded);
		str3[i] = expanded;
		i++;
	}
	return (str3);
}
