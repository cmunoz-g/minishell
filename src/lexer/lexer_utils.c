/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:41:01 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 13:41:02 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_qt(bool *quotes, char *quote_type, char *cmd_line, int i)
{
	(*quotes) = true;
	(*quote_type) = cmd_line[i];
}

void	init_lexer(bool *qt, int *i, int *start)
{
	*qt = false;
	*i = -1;
	*start = 0;
}
