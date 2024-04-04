/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 12:20:40 by juramos           #+#    #+#             */
/*   Updated: 2023/09/18 13:03:57 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	pos;

	pos = 0;
	if (n == 0)
		return (0);
	while (pos < n && s1[pos] != '\0' && s2[pos] != '\0')
	{
		if (s1[pos] != s2[pos])
			return ((unsigned char)s1[pos] - (unsigned char)s2[pos]);
		pos++;
	}
	if (n == pos)
		pos--;
	return ((unsigned char)s1[pos] - (unsigned char)s2[pos]);
}
