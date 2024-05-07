/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:18:16 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 16:18:30 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp("HOME=", envp[i], 5))
			return (envp[i]);
		i++;
	}
	return (NULL);
}

void	load_history(int fd)
{
	char	*line;
	char	*to_add;

	line = get_next_line(fd);
	if (line)
		to_add = ft_substr(line, 0, (ft_strlen(line) - 1));
	while (line)
	{
		add_history(to_add);
		free(to_add);
		free(line);
		line = get_next_line(fd);
		if (line)
			to_add = ft_substr(line, 0, (ft_strlen(line) - 1));
	}
}

void	get_past_history(char **envp, t_minishell *data)
{
	int		fd;
	char	*home;

	home = get_home(envp);
	if (home)
	{
		home += 5;
		home = ft_strjoin(home, "/.history");
		if (!home)
			error(data, "Memory problems when loading history");
		fd = open(home, O_CREAT | O_RDWR | O_APPEND, 0644);
		free(home);
		if (fd == -1)
		{
			error(data, "Could not load history");
			exit(EXIT_FAILURE);
		}
		load_history(fd);
		close(fd);
	}
}

void	join_history(char *cmd, t_minishell *data, char **envp)
{
	int		fd;
	char	*home;

	if (!cmd || !(*cmd))
		return ;
	home = get_home(envp);
	if (home)
	{
		home += 5;
		home = ft_strjoin(home, "/.history");
		if (!home)
			error(data, "Memory problems when adding to history");
		fd = open(home, O_CREAT | O_RDWR | O_APPEND, 0644);
		free(home);
		if (fd == -1)
		{
			error(data, "Could not add to history");
			exit(EXIT_FAILURE);
		}
		ft_putstr_fd(cmd, fd);
		ft_putchar_fd('\n', fd);
		close(fd);
	}
	add_history(cmd);
}
