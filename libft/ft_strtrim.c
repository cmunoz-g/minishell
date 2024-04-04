/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:04:21 by juramos           #+#    #+#             */
/*   Updated: 2023/10/07 14:05:41 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed;

	if (!s1 || !set)
		return (0);
	start = 0;
	while (start < ft_strlen(s1) && ft_strchr(set, s1[start]) != 0)
		start++;
	end = ft_strlen(s1) - 1;
	while (end >= start && ft_strchr(set, s1[end]) != 0)
		end--;
	trimmed = ft_substr(s1, start, end + 1 - start);
	if (!trimmed)
		return (0);
	return (trimmed);
}
/*
int	main(int argc, char **argv)
{
	char	*s;

	if (argc == 3)
	{
		s = ft_strtrim(argv[1], argv[2]);
		printf("ft_strtrim for %s, trimming %s is %s.\n",
			argv[1], argv[2], s);
		free(s);
	}
	system("leaks -q a.out");
}
*/
