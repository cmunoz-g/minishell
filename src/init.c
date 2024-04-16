#include "minishell.h"

t_minishell	*init(void)
{
	t_minishell *data;

	data = (t_minishell *)malloc(sizeof(t_minishell));
	// if (!data)
	// 		error ft
	data->token_list = NULL;
	data->cmd_table = NULL;
	// init env variables
	return (data);
}