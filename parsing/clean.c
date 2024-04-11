// #include "minishel.h"
#include "parsing_tests.h"

void	clean_token_list(t_token **token_list)
{
	t_token *tmp;

	if ((*token_list)->next_cmd)
		clean_token_list(&(*token_list)->next_cmd);
	while (*token_list)
	{
		tmp = (*token_list)->next;
		if ((*token_list)->value)
			free((*token_list)->value);
		free(*token_list);
		(*token_list) = tmp;
	}
}
void	clean_cmd_table_list(t_cmd_table **cmd_table)
{
	t_cmd_table *tmp;
	int			i;
	int			j;

	i = 0;
	j = 0;
	while (*cmd_table)
	{
		tmp = (*cmd_table)->next;
		while ((*cmd_table)->args[i])
			free((*cmd_table)->args[i++]);
		while (j < (*cmd_table)->n_redirections)
		{
			free((*cmd_table)->redirections[j]->value);
			free((*cmd_table)->redirections[j]);
			j++;
		}
		free((*cmd_table)->redirections);
		free((*cmd_table)->cmd);
		free(*cmd_table);
		i = 0;
		j = 0;
		*cmd_table = tmp;
	}
}


// typedef struct s_cmd_table
// {
// 	char				*cmd;
// 	char				**args;
// 	int					in;
// 	int					out;
// 	int 				err;
// 	t_token				**redirections;
// 	int					n_redirections;		
// 	bool				new_cmd;		
// 	struct s_cmd_table	*prev;
// 	struct s_cmd_table	*next;
// }						t_cmd_table;