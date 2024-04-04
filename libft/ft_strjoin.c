/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 13:05:07 by juramos           #+#    #+#             */
/*   Updated: 2023/10/03 12:13:48 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;
	int		pos;
	int		helper;

	pos = 0;
	helper = 0;
	if (!s1 || !s2)
		return (0);
	s3 = ft_calloc((ft_strlen(s1) + ft_strlen(s2) + 1), sizeof(char));
	if (!s3)
		return (0);
	while (s1[helper] != '\0')
		s3[pos++] = s1[helper++];
	helper = 0;
	while (s2[helper] != '\0')
		s3[pos++] = s2[helper++];
	return (s3);
}
/*
int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		printf("The output of ft_strjoin for %s and %s is %s.\n",
			argv[1], argv[2], ft_strjoin(argv[1], argv[2]));
	}
	return (0);
}
*/
