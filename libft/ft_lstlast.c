/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 17:05:52 by juramos           #+#    #+#             */
/*   Updated: 2023/09/25 17:15:28 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}
/*
int	main(void)
{
	char		*val;
	char		*val2;
	char		*val3;
	t_list	*t;
	t_list	*t2;
	t_list	*t3;

	val = "Goodbye";
	t = ft_lstnew(val);
	val2 = "Hello";
	t2 = ft_lstnew(val2);
	val3 = "Baby";
	t3 = ft_lstnew(val3);
	printf("value of t before: %s for content, %s for next.\n",
		(char *)(t->content), (char *)(t->next));
	printf("value of t2 before: %s for content, %s for next.\n",
		(char *)(t2->content), (char *)(t2->next));
	printf("value of t3 before: %s for content, %s for next.\n",
		(char *)(t3->content), (char *)(t3->next));
	printf("Length of t: %i. Length of t2: %i. Length of t3: %i\n",
		ft_lstsize(t), ft_lstsize(t2), ft_lstsize(t3));
	ft_lstadd_front(&t, t2);
	ft_lstadd_front(&t, t3);
	printf("value of t after: %s for content, %s for next.\n",
		(char *)(t->content), (char *)((t->next)->content));
	printf("value of t2 after: %s for content, %s for next.\n",
		(char *)(t2->content), (char *)((t2->next)->content));
	printf("value of t3 after: %s for content, %s for next.\n",
		(char *)(t3->content), (char *)((t3->next)->content));
	printf("Length of t: %i. Length of t2: %i. Length of t3: %i\n",
		ft_lstsize(t), ft_lstsize(t2), ft_lstsize(t3));
	printf("Last for t3 is %s.\n", (char *)((ft_lstlast(t3))->content));
	printf("Last for t2 is %s.\n", (char *)((ft_lstlast(t2))->content));
	printf("Last for t is %s.\n", (char *)((ft_lstlast(t))->content));
}
*/