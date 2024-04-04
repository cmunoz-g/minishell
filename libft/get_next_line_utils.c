/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 10:35:03 by juramos           #+#    #+#             */
/*   Updated: 2024/04/03 11:22:26 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_gnl_strchr(const char *s, int c)
{
	while (*s != (unsigned char)c)
	{
		if (!*s++)
			return (0);
	}
	return (1);
}
