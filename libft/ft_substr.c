/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 12:12:43 by juramos           #+#    #+#             */
/*   Updated: 2023/10/02 10:58:28 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;

	if (!s)
		return (0);
	if (start > ft_strlen(s))
		len = 0;
	else if (ft_strlen(s) - start < len)
		len = ft_strlen(s) - start;
	ret = malloc((len + 1) * sizeof(char));
	if (!ret)
	{
		free(ret);
		return (0);
	}
	if (start <= ft_strlen(s))
	{
		while (start--)
			s++;
	}
	ft_strlcpy(ret, s, len + 1);
	return (ret);
}
/*
int	main(int argc, char **argv)
{
	if (argc == 4)
	{
		printf("ft_substr of %s is %s, with start as %s and len %s.\n",
			argv[1], ft_substr(argv[1], ft_atoi(argv[2]), ft_atoi(argv[3])),
			argv[2], argv[3]);
	}
	return (0);
}
*/
