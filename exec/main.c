/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:40:49 by juramos           #+#    #+#             */
/*   Updated: 2024/04/08 12:27:08 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
	Por ahora solo gestionamos un arg
*/

void	print_cmd(t_cmd_table *tbl)
{
	int	i;
	if (!tbl)
		;
	i = 0;
	printf("cmd: %s\n", tbl->cmd);
	while (tbl->args[i++])
		printf("arg %d: %s\n", i, tbl->args[i]);
	printf("in: %d\n", tbl->in);
	printf("out: %d\n", tbl->out);
	printf("--------\n");
}

void	handle_cmd(t_cmd_table *tbl, char **envp)
{
	if (tbl)
		exit(0);
	if (tbl->redirections)
		if (redirect(tbl))
			exit(0);
	if (!tbl->next || tbl->out == FILE)
		exec_process(tbl, envp);
	else if (tbl->next)
		do_pipe(tbl, envp);
	exit(1);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd_table	*tbl;
	t_cmd_table	*tbl2;
	t_token		*redirections;

	if (argc != 1 || argv[1])
		exit(1);
	tbl = malloc(sizeof(t_cmd_table));
	if (!tbl)
		exit(0);
	tbl->cmd = "ls";
	tbl->args = ft_calloc(sizeof(char *), 2);
	tbl->args[0] = "-la";
	tbl->in = STDIN;
	tbl->out = PIPE;
	tbl->redirections = NULL;
	tbl->n_redirections = 0;
	tbl2 = malloc(sizeof(t_cmd_table));
	if (!tbl2)
		exit(0);
	redirections = malloc(sizeof(t_token));
	if (!redirections)
		exit(0);
	redirections->type = FILE;
	redirections->value = "out.txt";
	redirections->prev = NULL;
	redirections->next = NULL;
	tbl2->cmd = "grep";
	tbl->args = ft_calloc(sizeof(char *), 2);
	tbl->args[0] = "Make";
	tbl2->in = PIPE;
	tbl2->out = FILE;
	tbl->next = tbl2;
	tbl2->prev = tbl;
	tbl2->redirections = redirections;
	tbl2->n_redirections = 1;
	while (tbl)
	{
		print_cmd(tbl);
		handle_cmd(tbl, envp);
		tbl = tbl->next;
	}
	return (0);
}
