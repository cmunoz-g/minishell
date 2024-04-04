/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:59:03 by juramos           #+#    #+#             */
/*   Updated: 2023/10/02 14:07:22 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*last;

	last = 0;
	while (*s)
	{
		if (*s == (char)c)
			last = (char *)s;
		s++;
	}
	if (!c || *s == (char)c)
		last = (char *)s;
	return (last);
}
/*
#include <string.h>
int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		printf("value of strrchr: %s.\nvalue of ft_strrchr: %s.\n",
			strrchr(argv[1], ft_atoi(argv[2])),
			ft_strrchr(argv[1], ft_atoi(argv[2])));
	}
	return (0);
}
*/
