#include "minishell.h"

int	get_nbr_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

int	get_equal_sign(char *variable)
{
	int	i;

	i = 0;
	while (variable[i] && variable[i] != '=')
		i++;
	return (i);
}

int	is_env(char *variable, char **env)
{
	int	i;
	int	equal_pos;

	i = 0;
	equal_pos = 0;
	while (env[i])
	{
		equal_pos = get_equal_sign(env[i]);
		if (!ft_strncmp(variable, env[i], equal_pos)) // j o j+1?
			return (0);
		i++;
	}
	return (1);
}

char	**unset_env(char *variable, char **env, t_minishell *data) 
{
	char	**new_env;
	int		nbr_env;
	int		i;
	int		j;
	int		equal_pos;

	i = 0;
	j = 0;
	nbr_env = get_nbr_env(env);
	new_env = (char **)malloc(sizeof(char *) * (nbr_env - 1));
	if (!new_env)
		error(data, "Memory problems in mini_unset");
	while (env[i])
	{
		equal_pos = get_equal_sign(env[i]);
		if (ft_strncmp(variable, env[i], equal_pos))
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j]) // add line to free the new_env[j]
				error(data, "Memory problems in mini_unset");
			j++;
		}
		i++;
		free(env[i]);
	}
	new_env[j] = NULL;
	free(env);
	return (new_env);
}

void	mini_unset(t_minishell *data)
{
	int	i;

	i = 0;
	while (data->cmd_table->args[i])
	{
		if (!is_env(data->cmd_table->args[i], data->env_vars))
		{
			data->env_vars = unset_env(data->cmd_table->args[i], data->env_vars, data);
		}
		// Gestionar variables locales

		// else
		// {

		// }
		i++;
	}
}