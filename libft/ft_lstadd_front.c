/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:37:45 by juramos           #+#    #+#             */
/*   Updated: 2023/09/25 16:59:55 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
}
/*
int	main(void)
{
	char		*val;
	char		*val2;
	t_list	*t;
	t_list	*t2;

	val = "Goodbye";
	t = ft_lstnew(val);
	val2 = "Hello";
	t2 = ft_lstnew(val2);
	printf("value of t before: %s for content, %s for next.\n",
		(char *)(t->content), (char *)(t->next));
	printf("value of t2 before: %s for content, %s for next.\n",
		(char *)(t2->content), (char *)(t2->next));
	ft_lstadd_front(&t, t2);
	printf("value of t after: %s for content, %s for next.\n",
		(char *)(t->content), (char *)((t->next)->content));
	printf("value of t2 after: %s for content, %s for next.\n",
		(char *)(t2->content), (char *)((t2->next)->content));
}
*/