/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:11:14 by juramos           #+#    #+#             */
/*   Updated: 2024/05/01 13:34:30 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_heredoc_filename(void)
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

static int	create_hd_file(t_minishell *data, int redir)
{
	char	*line;
	char	*expanded;
	int		fd;

	line = readline(HEREDOC_MSG);
	fd = open(data->cmd_table->hd_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (1);
	while (line && ft_strncmp(line, data->cmd_table->redirections[redir]->value,
		ft_strlen(data->cmd_table->redirections[redir]->value)))
	{
		expanded = expand(line, 1, data);
		if (!expanded)
			return (EXIT_FAILURE);
		ft_putendl_fd(expanded, fd);
		free(expanded);
		line = readline(HEREDOC_MSG);
	}
	free(line);
	close(fd);
	return (0);
}

static int	check_heredocs(t_minishell *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_table->n_redirections)
	{
		if (data->cmd_table->redirections[i]->type == HEREDOC)
		{
			if (data->cmd_table->hd_file)
				free(data->cmd_table->hd_file);
			data->cmd_table->hd_file = get_heredoc_filename();
			if (!(data->cmd_table)->hd_file)
				return (1);
			if (create_hd_file(data, i))
				return (1);
		}
		i++;
	}
	return (0);
}

int	check_all_heredocs(t_minishell *data)
{
	t_cmd_table	*start;

	start = data->cmd_table;
	while (data->cmd_table)
	{
		if (check_heredocs(data))
			return (1);
		data->cmd_table = data->cmd_table->next;
	}
	data->cmd_table = start;
	return (0);
}
