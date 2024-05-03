/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:07:57 by juramos           #+#    #+#             */
/*   Updated: 2024/05/03 10:44:03 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global;

void	print_local_variables(t_variable *local_vars) // borrar
{
	t_variable *ref;

	ref = local_vars;
	while (ref)
	{
		printf("name: %s \nvalue: %s\n",ref->name, ref->value);
		ref = ref->next;
	}
}

void	print_tokens(t_token *token_list) // borrar
{
	while (token_list)
	{
	 	printf("value:%s type:%d\n",token_list->value, token_list->type);
	 	token_list = token_list->next;
	}
}

void	print_cmd_table(t_cmd_table *cmd_table) // borrar
{
	int i = 0; 

	while (cmd_table) 
	{
		printf("CMD:%s\n",cmd_table->cmd);
		
		if (cmd_table->args)
		{
			while (cmd_table->args[i])
			{
				printf("ARG%d:%s\n", i, cmd_table->args[i]);
				i++;
			}

		}
		if (cmd_table->in)
			printf("IN:%d\n", cmd_table->in);
		printf("OUT:%d\n", cmd_table->out);
		printf("ERR:%d\n", cmd_table->err);
		i = 0;
		while (i < cmd_table->n_redirections)
		{
			printf("redir number: %d type: %d value: %s\n", i, cmd_table->redirections[i]->type, cmd_table->redirections[i]->value);
			i++;
		}
		printf("nbr redir: %d\n", cmd_table->n_redirections);
		if (cmd_table->new_cmd)
			printf("new cdm TRUE\n");
		else 
			printf("new cdm FALSE\n");
		cmd_table = cmd_table->next;
		printf("\n");
	}
}

void leaks(void)
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	//atexit(leaks);
	if (argc != 1 || argv[1])
		exit(1);
	data = init(envp);
	get_past_history(envp, data);
	init_signals();
	minishell_loop(data);
}
