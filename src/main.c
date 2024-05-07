/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:07:57 by juramos           #+#    #+#             */
/*   Updated: 2024/05/07 16:18:44 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global;

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	if (argc != 1 || argv[1])
		exit(1);
	data = init(envp);
	get_past_history(envp, data);
	init_signals();
	minishell_loop(data);
}
