/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:40:49 by juramos           #+#    #+#             */
/*   Updated: 2024/04/05 12:45:21 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
	Por ahora solo gestionamos un arg
*/

void	print_cmd(t_cmd_table *tbl)
{
	if (!tbl)
		;
	printf("cmd: %s\n", tbl->cmd);
	printf("arg %s\n", tbl->arg);
	printf("in: %d\n", tbl->in);
	printf("out: %d\n", tbl->out);
	printf("filename: %s\n", tbl->filename);
	printf("--------\n");
}

void	handle_cmd(t_cmd_table *tbl)
{
	if (tbl)
		exit(0);
	exit(1);
}

int	main(void)
{
	t_cmd_table	*tbl;
	t_cmd_table	*tbl2;

	tbl = malloc(sizeof(t_cmd_table));
	if (!tbl)
		exit(0);
	tbl->cmd = "ls";
	tbl->arg = "-la";
	tbl->in = STDIN;
	tbl->out = PIPE;
	tbl2 = malloc(sizeof(t_cmd_table));
	if (!tbl2)
		exit(0);
	tbl2->cmd = "grep";
	tbl2->arg = "a";
	tbl2->in = PIPE;
	tbl2->out = FILE;
	tbl->next = tbl2;
	tbl2->prev = tbl;
	while (tbl)
	{
		print_cmd(tbl);
		handle_cmd(tbl);
		tbl = tbl->next;
	}
	return (0);
}
