#include "minishell.h"

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
void	clean_cmd_table_redir(t_cmd_table **cmd_table, int *j)
{
	if (!cmd_table || !(*cmd_table))
		return ;
	while (*j < (*cmd_table)->n_redirections)
	{
		free((*cmd_table)->redirections[*j]->value);
		(*cmd_table)->redirections[*j]->value = NULL;
		free((*cmd_table)->redirections[*j]);
		(*cmd_table)->redirections[*j] = NULL; 
		(*j)++;
	}
	free((*cmd_table)->redirections);
	(*cmd_table)->redirections = NULL; 
}

t_cmd_table	**get_first_node(t_cmd_table ***cmd)
{
	while ((**cmd)->prev)
		*cmd = &(**cmd)->prev;
	return (*cmd);
}

void	clean_cmd_table_list(t_cmd_table **cmd_table)
{
	t_cmd_table	*tmp;
	int			i;
	int			j;

	i = 0;
	j = 0;
	cmd_table = get_first_node(&cmd_table);
	while (*cmd_table)
	{
		tmp = (*cmd_table)->next;
		if ((*cmd_table)->args)
		{
			while ((*cmd_table)->args[i])
			{
				free((*cmd_table)->args[i]);
				(*cmd_table)->args[i] = NULL;
				i++;
			}
			free((*cmd_table)->args);
			(*cmd_table)->args = NULL;
		}
		if ((*cmd_table)->n_redirections)
			clean_cmd_table_redir(cmd_table, &j);
		if ((*cmd_table)->cmd)
		{
			free((*cmd_table)->cmd);
			(*cmd_table)->cmd = NULL;
		}
		if ((*cmd_table)->hd_file)
		{
			free((*cmd_table)->hd_file);
			(*cmd_table)->hd_file = NULL;
		}
		free(*cmd_table);
		i = 0;
		j = 0;
		*cmd_table = tmp;
	}
	(*cmd_table) = NULL;
}
void	clean_data(t_minishell **data)
{
	if ((*data)->line)
		free((*data)->line);
	if ((*data)->env_vars)
		free_arr((*data)->env_vars);
	if ((*data)->pwd)
		free((*data)->pwd);
	if ((*data)->old_pwd)
		free((*data)->old_pwd);
	clean_local_vars(&(*data)->local_vars);
	clean_cmd_table_list(&((*data)->cmd_table));
	rl_clear_history();
	free((*data));
}

void	get_first_variable(t_variable **var)
{
	while ((*var)->prev)
		*var = (*var)->prev;
}

void	clean_local_vars(t_variable **local_vars)
{
	t_variable *it;
	get_first_variable(local_vars);
	
	while ((*local_vars))
	{
		it = (*local_vars)->next;
		if ((*local_vars)->name)
			free((*local_vars)->name);
		((*local_vars)->name) = NULL;
		if ((*local_vars)->value)
			free((*local_vars)->value);
		((*local_vars)->value) = NULL;
		free(*local_vars);
		(*local_vars) = it;
	}
}