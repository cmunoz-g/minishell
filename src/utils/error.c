/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:26:51 by juramos           #+#    #+#             */
/*   Updated: 2024/05/07 11:03:46 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_builtins(t_minishell *data, int exit_code)
{
	if (data)
	{
		if (data->cmd_table)
			clean_cmd_table_list(&data->cmd_table);
		if (data->token_list)
			clean_token_list(&data->token_list);
		free(data);
	}
	return (exit_code);
}

void	error(t_minishell *data, char *error_message)
{
	size_t	msg_len;

	if (data)
		clean_data(&data);
	msg_len = ft_strlen(error_message);
	write(2, "Error: ", 8);
	write(2, error_message, msg_len);
	write(2, "\n", 1);
}
