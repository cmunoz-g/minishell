/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:40:49 by juramos           #+#    #+#             */
/*   Updated: 2024/04/15 12:05:09 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
	| cmd  | args     | in    | out   | n_redirections | redirections                    |
	|------|----------|-------|-------|----------------|---------------------------------|
	| ls   | ["-la"]  | STDIN | PIPE  | 0              | NULL                            |
	| grep | ["Make"] | PIPE  | TRUNC | 1              | {type: TRUNC, value: "out.txt"} |
*/
t_cmd_table	*get_example_1(void);
/*
	| cmd  | args         | in    | out    | n_redirections | redirections                     |
	|------|--------------|-------|--------|----------------|----------------------------------|
	| ls   | ["-la"]      | STDIN | PIPE   | 0              | NULL                             |
	| grep | ["Make"]     | PIPE  | PIPE   | 0              | NULL                             |
	| wc   | ["-w", "-l"] | PIPE  | APPEND | 1              | {type: APPEND, value: "out.txt"} |
*/
t_cmd_table	*get_example_2(void);
/*
	| cmd | args | in      | out   | n_redirections | redirections                                                     |
	|-----|------|---------|-------|----------------|------------------------------------------------------------------|
	| cat | NULL | HEREDOC | TRUNC | 2              | {{type: HEREDOC, value: "END"}, {type: TRUNC, value: "out.txt"}} |
*/
t_cmd_table	*get_example_3(void);

/*
	| cmd | args | in      | out   | n_redirections | redirections                                                      |
	|-----|------|---------|-------|----------------|------------------------------------------------------------------ |
	| cat | NULL | HEREDOC | TRUNC | 2              | {{type: INPUT, value: "in.txt"}, {type: TRUNC, value: "out.txt"}} |
*/
t_cmd_table	*get_example_4(void);
/*
	| cmd  | args                              | in    | out    | n_redirections | redirections |
	|------|-----------------------------------|-------|--------|----------------|--------------|
	| echo | "$PWD", "hi$USER", "$NOTEXISTING" | STDIN | PIPE   | 0              | NULL         |
	| cat   | "-e"                             | PIPE  | STDOUT | 0              | NULL         |
*/
t_cmd_table	*get_example_5(void);
/*
	| cmd  | args    | in      | out   | n_redirections | redirections                   |
	|------|---------|---------|-------|----------------|--------------------------------|
	| cat  | NULL    | HEREDOC | PIPE  | 1              | {{type: HEREDOC, value: "END"} |
	| echo | "hello" | PIPE    | PIPE  | 0              | NULL                           |
	| cat  | NULL    | HEREDOC | PIPE  | 1              | {{type: HEREDOC, value: "BYE"} |
*/
t_cmd_table	*get_example_6(void);

void	print_cmd(t_cmd_table *tbl)
{
	int	i;

	if (!tbl)
		;
	i = 0;
	printf("cmd: %s\n", tbl->cmd);
	while (tbl->args && tbl->args[i])
	{
		printf("arg %d: %s\n", i, tbl->args[i]);
		i++;
	}
	printf("in: %d\n", tbl->in);
	printf("out: %d\n", tbl->out);
	printf("--------\n");
}

void	handle_cmd(t_cmd_table *tbl, char **envp)
{
	if (!tbl)
		exit(0);
	if (tbl->n_redirections > 0)
		if (redirect(tbl))
			exit(0);
	if (!tbl->next || tbl->out == TRUNC || tbl->out == APPEND)
		exec_process(tbl, envp);
	else if (tbl->next)
		do_pipe(tbl, envp);
}

void	executor(t_cmd_table *tbl, char **envp)
{
	check_all_heredocs(tbl, envp);
	while (tbl)
	{
		print_cmd(tbl);
		handle_cmd(tbl, envp);
		tbl = tbl->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd_table	*tbl;

	if (argc != 1 || argv[1])
		exit(1);
	tbl = get_example_6();
	executor(tbl, envp);
	return (0);
}

/*
	| cmd  | args     | in    | out   | n_redirections | redirections                    |
	|------|----------|-------|-------|----------------|---------------------------------|
	| ls   | ["-la"]  | STDIN | PIPE  | 0              | NULL                            |
	| grep | ["Make"] | PIPE  | TRUNC | 1              | {type: TRUNC, value: "out.txt"} |
*/
t_cmd_table	*get_example_1(void)
{
	t_cmd_table	*tbl;
	t_cmd_table	*tbl2;
	t_token		*redirections;

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
	redirections = ft_calloc(sizeof(t_token), 2);
	if (!redirections)
		exit(0);
	redirections[0].type = TRUNC;
	redirections[0].value = "out.txt";
	tbl2->cmd = "grep";
	tbl2->args = ft_calloc(sizeof(char *), 2);
	tbl2->args[0] = "Make";
	tbl2->in = PIPE;
	tbl2->out = TRUNC;
	tbl->next = tbl2;
	tbl2->prev = tbl;
	tbl2->redirections = redirections;
	tbl2->n_redirections = 1;
	return (tbl);
}

/*
	| cmd  | args         | in    | out    | n_redirections | redirections                     |
	|------|--------------|-------|--------|----------------|----------------------------------|
	| ls   | ["-la"]      | STDIN | PIPE   | 0              | NULL                             |
	| grep | ["Make"]     | PIPE  | PIPE   | 0              | NULL                             |
	| wc   | ["-w", "-l"] | PIPE  | APPEND | 1              | {type: APPEND, value: "out.txt"} |
*/
t_cmd_table	*get_example_2(void)
{
	t_cmd_table	*tbl;
	t_cmd_table	*tbl2;
	t_cmd_table	*tbl3;
	t_token		*redirections;

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
	tbl2->cmd = "grep";
	tbl2->args = ft_calloc(sizeof(char *), 2);
	tbl2->args[0] = "Make";
	tbl2->in = PIPE;
	tbl2->out = PIPE;
	tbl2->redirections = NULL;
	tbl2->n_redirections = 0;
	tbl->next = tbl2;
	tbl2->prev = tbl;
	tbl3 = malloc(sizeof(t_cmd_table));
	if (!tbl3)
		exit(0);
	tbl3->cmd = "wc";
	tbl3->args = ft_calloc(sizeof(char *), 3);
	tbl3->args[0] = "-w";
	tbl3->args[1] = "-l";
	tbl3->in = PIPE;
	tbl3->out = APPEND;
	redirections = ft_calloc(sizeof(t_token *), 2);
	if (!redirections)
		exit(0);
	redirections[0].type = APPEND;
	redirections[0].value = "out.txt";
	tbl3->redirections = redirections;
	tbl3->n_redirections = 1;
	tbl2->next = tbl3;
	tbl3->prev = tbl2;
	return (tbl);
}
/*
	| cmd | args | in      | out   | n_redirections | redirections                                                     |
	|-----|------|---------|-------|----------------|------------------------------------------------------------------|
	| cat | NULL | HEREDOC | TRUNC | 2              | {{type: HEREDOC, value: "END"}, {type: TRUNC, value: "out.txt"}} |
*/
t_cmd_table	*get_example_3(void)
{
	t_cmd_table	*tbl;
	t_token		*redirections;

	tbl = malloc(sizeof(t_cmd_table));
	if (!tbl)
		exit(0);
	tbl->cmd = "cat";
	tbl->args = NULL;
	tbl->in = HEREDOC;
	tbl->out = TRUNC;
	tbl->n_redirections = 2;
	redirections = ft_calloc(sizeof(t_token), 3);
	if (!redirections)
		exit(0);
	redirections[0].type = HEREDOC;
	redirections[0].value = "END";
	redirections[1].type = TRUNC;
	redirections[1].value = "exec/out.txt";
	tbl->redirections = redirections;
	return (tbl);
}
/*
	| cmd | args | in      | out   | n_redirections | redirections                                                      |
	|-----|------|---------|-------|----------------|------------------------------------------------------------------ |
	| cat | NULL | INPUT   | TRUNC | 2              | {{type: INPUT, value: "in.txt"}, {type: TRUNC, value: "out.txt"}} |
*/
t_cmd_table	*get_example_4(void)
{
	t_cmd_table	*tbl;
	t_token		*redirections;

	tbl = malloc(sizeof(t_cmd_table));
	if (!tbl)
		exit(0);
	tbl->cmd = "cat";
	tbl->args = NULL;
	tbl->in = HEREDOC;
	tbl->out = TRUNC;
	tbl->n_redirections = 2;
	redirections = ft_calloc(sizeof(t_token), 3);
	if (!redirections)
		exit(0);
	redirections[0].type = INPUT;
	redirections[0].value = "exec/in.txt";
	redirections[1].type = TRUNC;
	redirections[1].value = "exec/out.txt";
	tbl->redirections = redirections;
	return (tbl);
}
/*
	| cmd  | args                              | in    | out    | n_redirections | redirections |
	|------|-----------------------------------|-------|--------|----------------|--------------|
	| echo | "$PWD", "hi$USER", "$NOTEXISTING" | STDIN | PIPE   | 0              | NULL         |
	| cat   | "-e"                             | PIPE  | STDOUT | 0              | NULL         |
*/
t_cmd_table	*get_example_5(void)
{
	t_cmd_table	*tbl;
	t_cmd_table	*tbl2;

	tbl = malloc(sizeof(t_cmd_table));
	if (!tbl)
		exit(0);
	tbl->cmd = "echo";
	tbl->args = ft_calloc(sizeof(char *), 4);
	tbl->args[0] = ft_calloc(sizeof(char), 7);
	tbl->args[0][0] = '\"';
	tbl->args[0][1] = '$';
	tbl->args[0][2] = 'P';
	tbl->args[0][3] = 'W';
	tbl->args[0][4] = 'D';
	tbl->args[0][5] = '\"';
	tbl->args[1] = ft_calloc(sizeof(char), 10);
	tbl->args[1][0] = '\"';
	tbl->args[1][1] = 'h';
	tbl->args[1][2] = 'i';
	tbl->args[1][3] = '$';
	tbl->args[1][4] = 'U';
	tbl->args[1][5] = 'S';
	tbl->args[1][6] = 'E';
	tbl->args[1][7] = 'R';
	tbl->args[1][8] = '\"';
	tbl->args[2] = ft_calloc(sizeof(char), 15);
	tbl->args[2][0] = '\"';
	tbl->args[2][1] = '$';
	tbl->args[2][2] = 'N';
	tbl->args[2][3] = 'O';
	tbl->args[2][4] = 'T';
	tbl->args[2][5] = 'E';
	tbl->args[2][6] = 'X';
	tbl->args[2][7] = 'I';
	tbl->args[2][8] = 'S';
	tbl->args[2][9] = 'T';
	tbl->args[2][10] = 'I';
	tbl->args[2][11] = 'N';
	tbl->args[2][12] = 'G';
	tbl->args[2][13] = '\"';
	tbl->in = STDIN;
	tbl->out = PIPE;
	tbl->redirections = NULL;
	tbl->n_redirections = 0;
	tbl2 = malloc(sizeof(t_cmd_table));
	if (!tbl2)
		exit(0);
	tbl2->cmd = "cat";
	tbl2->args = ft_calloc(sizeof(char *), 2);
	tbl2->args[0] = "-e";
	tbl2->in = PIPE;
	tbl2->out = STDOUT;
	tbl2->redirections = NULL;
	tbl2->n_redirections = 0;
	tbl->next = tbl2;
	tbl2->prev = tbl;
	return (tbl);
}
/*
	| cmd  | args    | in      | out   | n_redirections | redirections                   |
	|------|---------|---------|-------|----------------|--------------------------------|
	| cat  | NULL    | HEREDOC | PIPE  | 1              | {{type: HEREDOC, value: "END"} |
	| echo | "hello" | PIPE    | PIPE  | 0              | NULL                           |
	| cat  | NULL    | HEREDOC | PIPE  | 1              | {{type: HEREDOC, value: "BYE"} |
*/
t_cmd_table	*get_example_6(void)
{
	t_cmd_table	*tbl;
	t_cmd_table	*tbl2;
	t_cmd_table	*tbl3;
	t_token		*redirections;
	t_token		*redirections2;

	tbl = malloc(sizeof(t_cmd_table));
	if (!tbl)
		exit(0);
	tbl->cmd = "cat";
	tbl->args = NULL;
	tbl->in = HEREDOC;
	tbl->out = PIPE;
	tbl->n_redirections = 1;
	redirections = ft_calloc(sizeof(t_token), 2);
	if (!redirections)
		exit(0);
	redirections[0].type = HEREDOC;
	redirections[0].value = "END";
	tbl->redirections = redirections;
	tbl2 = malloc(sizeof(t_cmd_table));
	if (!tbl2)
		exit(0);
	tbl2->cmd = "echo";
	tbl2->args = ft_calloc(sizeof(char **), 2);
	tbl2->args[0] = ft_calloc(sizeof(char *), 6);
	tbl2->args[0][0] = 'H';
	tbl2->args[0][1] = 'e';
	tbl2->args[0][2] = 'l';
	tbl2->args[0][3] = 'l';
	tbl2->args[0][4] = 'o';
	tbl2->in = PIPE;
	tbl2->out = PIPE;
	tbl2->n_redirections = 0;
	tbl2->redirections = NULL;
	tbl2->prev = tbl;
	tbl->next = tbl2;
	tbl3 = malloc(sizeof(t_cmd_table));
	if (!tbl3)
		exit(0);
	tbl3->cmd = "cat";
	tbl3->args = NULL;
	tbl3->in = HEREDOC;
	tbl3->out = STDOUT;
	tbl3->n_redirections = 1;
	redirections2 = ft_calloc(sizeof(t_token), 2);
	if (!redirections2)
		exit(0);
	redirections2[0].type = HEREDOC;
	redirections2[0].value = "BYE";
	tbl3->redirections = redirections2;
	tbl3->prev = tbl2;
	tbl2->next = tbl3;
	return (tbl);
}