/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:30:02 by juramos           #+#    #+#             */
/*   Updated: 2023/10/07 12:56:30 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char	*dst, const char *src, size_t n)
{
	size_t	pos;
	size_t	len;

	if (n == 0)
		return (ft_strlen(src));
	if (n <= ft_strlen(dst))
		return (n + ft_strlen(src));
	pos = 0;
	while (dst[pos] != '\0' && pos < n)
		pos++;
	len = ft_strlcpy(&dst[pos], src, n - pos);
	return (len + pos);
}
/*
int	main(int argc, char **argv)
{
	char	*s1 = argv[1];
	char	*s2 = argv[2];
	int		n = ft_atoi(argv[3]);
	int		n2;
	
	 char b[0xF] = "nyan !";
	 ft_strlcat(((void *)0), b, 2);

	// char b2[0xF] = "nyan !";
	// strlcat(((void *)0), b2, 2);

	if (argc == 4)
	{
		printf("s1 is %s. s2 is %s. n is %i.\n",
			s1, s2, n);
		n2 = ft_strlcat(s1, s2, n);
		printf("s1 is %s. s2 is %s. n is %i.\n",
			s1, s2, n2);
	}
}
*/
