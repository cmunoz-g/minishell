#include "minishell.h"

void	error_builtins(t_minishell *data, int exit_code)
{
	if (data)
	{
		if (data->cmd_table)
			clean_cmd_table_list(&data->cmd_table);
		if (data->token_list)
			clean_token_list(&data->token_list);
		free(data);
	}
	exit(exit_code);
}

void	error(t_minishell *data, char *error_message) // mirar como estan los errores gestionados en pipex e implementar
{
	size_t	msg_len;

	if (data)
		clean_data(&data);
	msg_len = ft_strlen(error_message);
	write(2, "Error: " , 8);
	write(2, error_message, msg_len);
	write(2, "\n", 1);
}