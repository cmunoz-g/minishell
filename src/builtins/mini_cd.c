/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:13:47 by juramos           #+#    #+#             */
/*   Updated: 2024/04/18 12:56:42 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_path_on_mini(t_minishell *data)
{
	char	*tmp;

	tmp = data->pwd;
	free(data->old_pwd);
	data->old_pwd = tmp;
	free(data->pwd);
	data->pwd = getcwd(NULL, sizeof(NULL));
}

static void	update_path_on_env(t_minishell *data)
{
	int	i;

	i = 0;
	while (data->env_vars[i])
	{
		if (!ft_strncmp(data->env_vars[i], "PWD=", ft_strlen("PWD=")))
		{
			free(data->env_vars[i]);
			data->env_vars[i] = data->pwd;
		}
		else if (
			!ft_strncmp(data->env_vars[i], "OLDPWD=", ft_strlen("OLDPWD=")))
		{
			free(data->env_vars[i]);
			data->env_vars[i] = data->old_pwd;
		}
		i++;
	}
}

int	mini_cd(t_minishell	*data)
{
	int	ret;

	if (!data->cmd_table->args[0])
		ret = chdir(my_getenv("HOME", data->env_vars));
	else
		ret = chdir(data->cmd_table->args[0]);
	if (ret != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(data->cmd_table->args[0], STDERR_FILENO);
		perror(" ");
		return (EXIT_FAILURE);
	}
	change_path_on_mini(data);
	update_path_on_env(data);
	return (EXIT_SUCCESS);
}
