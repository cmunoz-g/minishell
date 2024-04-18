/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:17:12 by juramos           #+#    #+#             */
/*   Updated: 2024/04/18 12:08:29 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (ft_strncmp(key, comp, ft_strlen(key)) == 0)
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
