/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:07:57 by juramos           #+#    #+#             */
/*   Updated: 2024/05/27 09:14:15 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global;

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	if (argc != 1 || argv[1])
		exit(1);
	if (!(*envp) || !envp)
	{
		printf("Error: No environment variables\n");
		exit(EXIT_FAILURE);
	}
	data = init(envp);
	get_past_history(envp, data);
	init_signals();
	minishell_loop(data);
}
