/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:13:47 by juramos           #+#    #+#             */
/*   Updated: 2024/05/15 13:14:59 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static void	change_path_on_mini(t_minishell *data, char *buff);
static void	update_path_on_env(t_minishell *data);

static void	change_path_on_mini(t_minishell *data, char *buff)
{
	char	*tmp;

	tmp = ft_strdup(data->pwd);
	if (data->old_pwd)
	{
		free(data->old_pwd);
		data->old_pwd = NULL;
	}
	data->old_pwd = tmp;
	free(data->pwd);
	data->pwd = getcwd(buff, PATH_MAX);
}

static void	update_path_on_env(t_minishell *data)
{
	int		i;
	char	*joined;

	i = 0;
	while (data->env_vars[i])
	{
		if (!ft_strncmp(data->env_vars[i], "PWD=", ft_strlen("PWD=")))
		{
			joined = ft_strjoin("PWD=", data->pwd);
			if (!joined)
				exit(1);
			free(data->env_vars[i]);
			data->env_vars[i] = joined;
		}
		else if (
			!ft_strncmp(data->env_vars[i], "OLDPWD=", ft_strlen("OLDPWD=")))
		{
			joined = ft_strjoin("OLDPWD=", data->old_pwd);
			if (!joined)
				exit(1);
			free(data->env_vars[i]);
			data->env_vars[i] = joined;
		}
		i++;
	}
}

static int	get_ret(t_minishell *data)
{
	int		ret;
	char	*expanded;

	if (!data->cmd_table->args[0])
		ret = chdir(my_getenv("HOME", data->env_vars));
	else
	{
		expanded = expand(data->cmd_table->args[0], 0, data);
		if (!expanded)
			return (EXIT_FAILURE);
		ret = chdir(expanded);
		free(expanded);
	}
	return (ret);
}

int	mini_cd(t_minishell	*data)
{
	int		ret;
	char	*buff;

	ret = get_ret(data);
	buff = ft_calloc(PATH_MAX, sizeof(char));
	if (ret != 0 || !getcwd(buff, PATH_MAX))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(data->cmd_table->args[0], STDERR_FILENO);
		perror(" ");
		free(buff);
		return (EXIT_FAILURE);
	}
	change_path_on_mini(data, buff);
	update_path_on_env(data);
	return (EXIT_SUCCESS);
}
