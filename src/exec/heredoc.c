/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:11:14 by juramos           #+#    #+#             */
/*   Updated: 2024/05/06 10:20:45 by juramos          ###   ########.fr       */
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

static int	create_hd_file(t_minishell *data, int redir, char *delim)
{
	char	*line;
	char	*expanded;
	int		fd;

	line = readline(HEREDOC_MSG);
	fd = open(data->cmd_table->hd_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (EXIT_FAILURE);
	while (line && ft_strncmp(line, delim, ft_strlen(delim))
		&& !g_global.stop_heredoc)
	{
		expanded = expand(line, 1, data);
		if (!expanded)
			return (EXIT_FAILURE);
		ft_putendl_fd(expanded, fd);
		free(expanded);
		free(line);
		line = readline(HEREDOC_MSG);
	}
	if (line)
		free(line);
	close(fd);
	if (g_global.stop_heredoc || !line)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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
				return (EXIT_FAILURE);
			g_global.stop_heredoc = 0;
			g_global.in_heredoc = 1;
			if (create_hd_file(data, i,
					data->cmd_table->redirections[i]->value))
				return (EXIT_FAILURE);
			g_global.in_heredoc = 0;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	check_all_heredocs(t_minishell *data)
{
	t_cmd_table	*start;

	start = data->cmd_table;
	while (data->cmd_table)
	{
		if (check_heredocs(data))
			return (EXIT_FAILURE);
		data->cmd_table = data->cmd_table->next;
	}
	data->cmd_table = start;
	return (EXIT_SUCCESS);
}
