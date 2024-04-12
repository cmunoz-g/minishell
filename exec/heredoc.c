/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:11:14 by juramos           #+#    #+#             */
/*   Updated: 2024/04/12 11:00:57 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static char	*get_heredoc_filename(void)
{
	static int	i = 0;
	char		*n;
	char		*filename;

	n = ft_itoa(i++);
	filename = ft_strjoin("exec/build/tmp_file_", n);
	if (!filename)
		return (NULL);
	free(n);
	return (filename);
}

static int	create_hd_file(char *filename, char *eof, char **envp)
{
	char	*line;
	char	*expanded;
	int		fd;

	line = readline(HEREDOC_MSG);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (1);
	while (line && ft_strncmp(line, eof, ft_strlen(eof)))
	{
		expanded = expand(line, 1, envp);
		ft_putendl_fd(expanded, fd);
		free(expanded);
		free(line);
		line = readline(HEREDOC_MSG);
	}
	free(line);
	close(fd);
	return (0);
}

int	check_heredocs(t_cmd_table *tbl, char **envp)
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
			if (create_hd_file(tbl->hd_file, tbl->redirections[i].value, envp))
				return (1);
		}
		i++;
	}
	return (0);
}
