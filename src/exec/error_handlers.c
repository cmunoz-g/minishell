/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 12:54:27 by juramos           #+#    #+#             */
/*   Updated: 2024/05/05 17:53:37 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	send_to_stderr(char *co, char *ar, char *err, int is_out)
{
	if (!co && is_out)
		return ;
	ft_putstr_fd("minishell: ", 2);
	if (ft_strncmp(err, "command not found", 18) == 0)
	{
		ft_putstr_fd(err, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(co, 2);
		return ;
	}
	if (co)
	{
		ft_putstr_fd(co, 2);
		ft_putstr_fd(": ", 2);
	}
	if (ar)
	{
		ft_putstr_fd(ar, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(err, 2);
}
