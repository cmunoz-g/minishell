/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:02:55 by juramos           #+#    #+#             */
/*   Updated: 2024/05/11 15:01:53 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_infile(char *str);
static int	handle_outfile(t_token *tkn);
static int	stderr_and_ret(char *str, int is_out);

static int	handle_outfile(t_token *tkn)
{
	int	fd;

	if (tkn->type == TRUNC)
		fd = open(tkn->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (tkn->type == APPEND)
		fd = open(tkn->value, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
		return (EXIT_FAILURE);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}

static int	handle_infile(char *str)
{
	int	fd;

	fd = open(str, O_RDONLY, 0777);
	if (fd < 0)
		return (EXIT_FAILURE);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}

static int	stderr_and_ret(char *str, int is_out)
{
	send_to_stderr(NULL, str, strerror(errno), is_out);
	return (errno);
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
				return (stderr_and_ret(tbl->redirections[i]->value, 1));
		}
		else if (tbl->redirections[i]->type == INPUT)
		{
			if (handle_infile(tbl->redirections[i]->value))
				return (stderr_and_ret(tbl->redirections[i]->value, 0));
		}
		else if (tbl->redirections[i]->type == HEREDOC && !is_builtin)
		{
			if (handle_infile(tbl->hd_file))
				return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

void	redirect_all(t_minishell *data)
{
	int	ret;

	if (data->cmd_table->n_redirections > 0)
	{
		ret = redirect(data->cmd_table, 0);
		if (ret)
		{
			g_global.error_num = ret;
			exit(ret);
		}
	}
}
