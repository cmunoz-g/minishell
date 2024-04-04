/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 10:49:40 by juramos           #+#    #+#             */
/*   Updated: 2023/09/18 13:05:42 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(char *haystack, char *needle, size_t len)
{
	size_t	h_pos;
	size_t	n_pos;
	size_t	needle_length;

	h_pos = 0;
	needle_length = ft_strlen(needle);
	if (needle_length == 0)
		return (haystack);
	while (h_pos < len && haystack[h_pos] != '\0')
	{
		n_pos = 0;
		while (haystack[h_pos + n_pos] == needle[n_pos]
			&& needle[n_pos] != '\0' && h_pos + n_pos < len)
			n_pos++;
		if (n_pos == needle_length)
			return (&haystack[h_pos]);
		h_pos++;
	}
	return (0);
}
