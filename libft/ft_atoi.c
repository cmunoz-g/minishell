/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 16:28:24 by juramos           #+#    #+#             */
/*   Updated: 2023/09/26 13:31:24 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(int c)
{
	if (c == ' ' || c == '\n'
		|| c == '\f' || c == '\r'
		|| c == '\t' || c == '\v')
		return (1);
	return (0);
}

static const char	*remove_whitespaces(const char *str)
{
	int	pos;

	pos = 0;
	while (ft_isspace(str[pos]))
		pos++;
	return (&str[pos]);
}

int	ft_atoi(const char *str)
{
	int	sign;
	int	num;
	int	pos;

	str = remove_whitespaces(str);
	pos = 0;
	num = 0;
	sign = 1;
	if (str[0] == '+')
		pos++;
	else if (str[0] == '-')
	{
		sign *= -1;
		pos++;
	}
	while (str[pos] != '\0')
	{
		if (ft_isdigit(str[pos]))
			num = num * 10 + str[pos] - 48;
		else
			return (num * sign);
		pos++;
	}
	return (num * sign);
}
