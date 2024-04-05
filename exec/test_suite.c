/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_suite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:40:49 by juramos           #+#    #+#             */
/*   Updated: 2024/04/05 09:36:45 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_suite.h"

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
	printf("--------\n");
}

t_cmd_table	*init_cmd(char *cmd, char *arg, int in, int out)
{
	t_cmd_table	*t;

	t = malloc(sizeof(t_cmd_table));
	if (!t)
		exit(0);
	t->cmd = malloc(sizeof(char) * ft_strlen(cmd));
	strcpy(t->cmd, cmd);
	t->arg = malloc(sizeof(char) * ft_strlen(arg));
	strcpy(t->arg, arg);
	t->in = in;
	t->out = out;
	t->next = NULL;
	t->prev = NULL;
	return (t);
}

int	main(void)
{
	t_cmd_table	*tbl;
	t_cmd_table	*tbl_2;

	tbl = init_cmd("ls", "-la", STDIN, PIPE);
	tbl_2 = init_cmd("grep", "a", PIPE, FILE);
	tbl->next = tbl_2;
	tbl_2->prev = tbl;
	while (tbl)
	{
		print_cmd(tbl);
		tbl = tbl->next;
	}
	return (0);
}
