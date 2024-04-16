/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 12:54:27 by juramos           #+#    #+#             */
/*   Updated: 2024/04/16 11:33:21 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	send_to_stderr(char *co, char *ar, char *err)
{
	char	**tmp;

	ft_putstr_fd("minishell: ", 2);
	if (ft_strncmp(err, "command not found", 18) == 0)
	{
		ft_putstr_fd(err, 2);
		ft_putstr_fd(": ", 2);
		tmp = ft_split(co, ' ');
		ft_putendl_fd(tmp[0], 2);
		free_split(tmp);
		return ;
	}
	if (co)
	{
		tmp = ft_split(co, ' ');
		ft_putstr_fd(tmp[0], 2);
		ft_putstr_fd(": ", 2);
		free_split(tmp);
	}
	if (ar)
	{
		ft_putstr_fd(ar, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(err, 2);
}
