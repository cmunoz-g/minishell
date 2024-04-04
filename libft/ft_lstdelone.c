/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 17:50:01 by juramos           #+#    #+#             */
/*   Updated: 2023/10/03 12:15:44 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst && del)
	{
		(*del)(lst->content);
		free(lst);
	}
}
/*
void	delone(void	*s)
{
	free(s);
}

int	main(void)
{
	char		*val;
	t_list	*t;

	val = "Goodbye";
	t = ft_lstnew(val);
	
	// for us to be able to free a pointer,
	// we must have allocated it first through malloc.
	// so, it's better to use malloc + strcpy.
	char *val2 = malloc(6);
	strcpy(val2, "Hello");
	t_list *t2 = ft_lstnew(val2);

	printf("value of t before: %s for content, %s for next.\n",
		(char *)(t->content), (char *)(t->next));
	printf("value of t2 before: %s for content, %s for next.\n",
		(char *)(t2->content), (char *)(t2->next));
	ft_lstadd_back(&t, t2);
	printf("value of t after: %s for content, %s for next.\n",
		(char *)(t->content), (char *)((t->next)->content));
	printf("value of t2 after: %s for content, %s for next.\n",
		(char *)(t2->content), (char *)(t2->next));
	ft_lstdelone(t2, delone);
	printf("value of t after: %s for content, %s for next.\n",
		(char *)(t->content), (char *)(t->next));
}
*/