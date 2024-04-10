/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:40:49 by juramos           #+#    #+#             */
/*   Updated: 2024/04/10 11:58:20 by juramos          ###   ########.fr       */
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
	| cmd  | args     | in    | out    | n_redirections | redirections |
	|------|----------|-------|--------|----------------|--------------|
	| ls   | ["-la"]  | STDIN | PIPE   | 0              | NULL         |
	| grep | ["Make"] | PIPE  | PIPE   | 0              | NULL         |
	| wc   | ["-w"]   | PIPE  | STDOUT | 0              | NULL         |
*/
t_cmd_table	*get_example_2(void);
/*
	| cmd | args | in      | out   | n_redirections | redirections                                                     |
	|-----|------|---------|-------|----------------|------------------------------------------------------------------|
	| cat | NULL | HEREDOC | TRUNC | 2              | {{type: HEREDOC, value: "END"}, {type: TRUNC, value: "out.txt"}} |
*/
t_cmd_table	*get_example_3(void);

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

char	*get_heredoc_filename(void)
{
	static int	i = 0;
	char		*n;
	char		*filename;

	n = ft_itoa(i++);
	filename = ft_strjoin("build/tmp_file_", n);
	if (!filename)
		return (NULL);
	free(n);
	return (filename);
}

int	create_hd_file(char *filename, char *eof)
{
	char	*line;
	int		fd;

	line = readline(HEREDOC_MSG);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	printf("filename is %s, fd is %d\n", filename, fd);
	ft_putendl_fd("HOLA", 1);
	if (fd < 0)
		return (1);
	while (line && ft_strncmp(line, eof, ft_strlen(eof)))
	{
		ft_putendl_fd(line, fd);
		free(line);
		line = readline(HEREDOC_MSG);
	}
	free(line);
	close(fd);
	return (0);
}

int	check_heredocs(t_cmd_table *tbl)
{
	int	i;

	i = 0;
	while (i < tbl->n_redirections)
	{
		if (tbl->redirections[i].type == HEREDOC)
		{
			if (tbl->hd_file)
				free(tbl->hd_file);
			tbl->hd_file = get_heredoc_filename();
			if (!tbl->hd_file)
				return (1);
			if (create_hd_file(tbl->hd_file, tbl->redirections[i].value))
				return (1);
		}
		i++;
	}
	return (0);
}

void	handle_cmd(t_cmd_table *tbl, char **envp)
{
	if (!tbl)
		exit(0);
	check_heredocs(tbl);
	if (tbl->redirections)
		if (redirect(tbl))
			exit(0);
	if (!tbl->next || tbl->out == TRUNC || tbl->out == APPEND)
		exec_process(tbl, envp);
	else if (tbl->next)
		do_pipe(tbl, envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd_table	*tbl;

	if (argc != 1 || argv[1])
		exit(1);
	tbl = get_example_3();
	while (tbl && envp)
	{
		print_cmd(tbl);
		handle_cmd(tbl, envp);
		tbl = tbl->next;
	}
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
	| cmd  | args     | in    | out    | n_redirections | redirections |
	|------|----------|-------|--------|----------------|--------------|
	| ls   | ["-la"]  | STDIN | PIPE   | 0              | NULL         |
	| grep | ["Make"] | PIPE  | PIPE   | 0              | NULL         |
	| wc   | ["-w"]   | PIPE  | STDOUT | 0              | NULL         |
*/
t_cmd_table	*get_example_2(void)
{
	t_cmd_table	*tbl;
	t_cmd_table	*tbl2;
	t_cmd_table	*tbl3;

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
	tbl3->args = ft_calloc(sizeof(char *), 2);
	tbl3->args[0] = "-w";
	tbl3->in = PIPE;
	tbl3->out = STDOUT;
	tbl3->redirections = NULL;
	tbl3->n_redirections = 0;
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
	redirections[1].value = "out.txt";
	tbl->redirections = redirections;
	return (tbl);
}
