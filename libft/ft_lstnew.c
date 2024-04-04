/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:18:27 by juramos           #+#    #+#             */
/*   Updated: 2023/09/25 13:35:22 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*ret;

	ret = malloc(sizeof(t_list));
	if (!ret)
		return (0);
	ret->content = content;
	ret->next = 0;
	return (ret);
}
/*
int	main(void)
{
	char		*val;
	t_list	*x;

	val = "Hello";
	x = ft_lstnew(val);
	printf("content: %s, next: %s.\n", (char *)(x->content), (char *)(x->next));
}
*/