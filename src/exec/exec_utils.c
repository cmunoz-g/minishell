/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:17:12 by juramos           #+#    #+#             */
/*   Updated: 2024/05/10 18:12:33 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_n_of_pipes(t_minishell *data)
{
	int			n;
	t_cmd_table	*start;

	n = 0;
	start = data->cmd_table;
	while (data->cmd_table->next && ++n)
		data->cmd_table = data->cmd_table->next;
	data->cmd_table = start;
	return (n);
}

int	ft_fork(t_minishell *data)
{
	static int	i = 0;

	if (g_global.reset_pipes)
	{
		g_global.reset_pipes = 0;
		i = 0;
	}
	data->pids[i] = fork();
	i++;
	return (i - 1);
}

int	open_file(char *name, int to_write)
{
	int	ret;

	if (to_write == 0)
		ret = open(name, O_RDONLY, 0777);
	if (to_write == 1)
		ret = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (to_write == 2)
		ret = open(name, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (ret == -1)
		exit(EXIT_FAILURE);
	return (ret);
}

char	*my_getenv(char *key, char **env)
{
	char	*comp;
	int		i;
	int		j;

	i = -1;
	while (env[++i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		comp = ft_substr(env[i], 0, j);
		if (!ft_strncmp(key, comp, ft_strlen(comp))
			&& !ft_strncmp(key, comp, ft_strlen(key)))
		{
			free(comp);
			return (&env[i][j + 1]);
		}
		free(comp);
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*exec;
	char	**env_vars;
	int		i;

	env_vars = ft_split(my_getenv("PATH", env), ':');
	if (!env_vars)
		return (NULL);
	i = -1;
	while (env_vars[++i])
	{
		path = ft_strjoin(env_vars[i], "/");
		exec = ft_strjoin(path, cmd);
		free(path);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_arr(env_vars);
			return (exec);
		}
		free(exec);
	}
	free_arr(env_vars);
	return (NULL);
}
