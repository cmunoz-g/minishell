/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:58:14 by juramos           #+#    #+#             */
/*   Updated: 2023/10/02 11:23:49 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_int_size(int n)
{
	int	size;

	size = 1;
	while ((n / 10 > 0 && n > 0)
		|| (n / 10 < 0 && n < 0))
	{
		n /= 10;
		size++;
	}
	return (size);
}

static int	pow_10(int size)
{
	int	result;

	result = 1;
	while (--size)
		result *= 10;
	return (result);
}

static int	divide_by_ten(int *n, int *size)
{
	int	ret;

	ret = *n / pow_10(*size);
	*n %= pow_10(*size);
	*size -= 1;
	if (ret < 0)
		ret *= -1;
	return (ret);
}

char	*ft_itoa(int n)
{
	int		size;
	int		len;
	int		pos;
	char	*ret;

	size = get_int_size(n);
	len = size;
	if (n < 0)
		len++;
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
	{
		free(ret);
		return (0);
	}
	pos = 0;
	if (n < 0)
		ret[pos++] = '-';
	while (pos < len)
		ret[pos++] = (char)(divide_by_ten(&n, &size)) + 48;
	ret[pos] = '\0';
	return (ret);
}
/*
int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		printf("Value of itoa for %i is %s.\n",
			ft_atoi(argv[1]), ft_itoa(ft_atoi(argv[1])));
	}
	return (0);
}
*/
