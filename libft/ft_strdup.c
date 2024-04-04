/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 12:03:14 by juramos           #+#    #+#             */
/*   Updated: 2023/09/19 12:07:26 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s2;
	size_t	s1_len;

	s1_len = ft_strlen(s1);
	s2 = (char *) malloc(s1_len * sizeof(char) + 1);
	if (!s2)
	{
		free(s2);
		return (0);
	}
	ft_strlcpy(s2, s1, s1_len + 1);
	return (s2);
}
