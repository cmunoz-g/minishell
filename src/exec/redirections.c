/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:02:55 by juramos           #+#    #+#             */
/*   Updated: 2024/04/24 12:43:58 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_outfile(t_token *tkn)
{
	int	fd;

	if (tkn->type == TRUNC)
		fd = open(tkn->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (tkn->type == APPEND)
		fd = open(tkn->value, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
		return (1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (1);
	close(fd);
	return (0);
}

static int	handle_infile(char *str)
{
	int	fd;

	fd = open(str, O_RDONLY, 0777);
	if (fd < 0)
		return (1);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (1);
	close(fd);
	return (0);
}

int	redirect(t_cmd_table *tbl, int is_builtin)
{
	int	i;

	i = 0;
	while (i < tbl->n_redirections)
	{
		if (tbl->redirections[i]->type == TRUNC
			|| tbl->redirections[i]->type == APPEND)
		{
			if (handle_outfile(tbl->redirections[i]))
				return (1);
		}
		else if (tbl->redirections[i]->type == INPUT)
		{
			if (handle_infile(tbl->redirections[i]->value))
				return (1);
		}
		else if (tbl->redirections[i]->type == HEREDOC && !is_builtin)
		{
			if (handle_infile(tbl->hd_file))
				return (1);
		}
		i++;
	}
	return (0);
}
