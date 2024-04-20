#include "minishell.h"

void	mini_env(t_minishell *data)
{
	int	i;

	i = 0;
	while (data->env_vars[i])
	{
		printf("%s\n",data->env_vars[i]);
		i++;
	}
}