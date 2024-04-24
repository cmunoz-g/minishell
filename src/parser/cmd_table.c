#include "minishell.h"

void	gen_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int start, int end)
{
	t_cmd_table *last;
	int			i;
	int			j;

	last = get_last_cmd_table((*cmd_table));
	i = 0;
	while (i < start)
	{
		token_list = token_list->next;
		i++;
	}
	last->args = (char **)malloc(sizeof(char *) * (get_nbr_args(token_list, (end - start)) + 1)); // FALTA PROTEGER
	last->n_redirections = get_nbr_redir(token_list, (end - start)); 
	if (last->n_redirections)
		last->redirections = (t_token **)malloc(sizeof(t_token *) * last->n_redirections); // FALTA PROTEGER
	else
		last->redirections = NULL;
	j = 0;
	while (j < last->n_redirections)
	{
		last->redirections[j] = (t_token *)malloc(sizeof(t_token)); // FALTA PROTEGER
		j++;
	}
	populate_cmd_table(token_list, &last, (end - start));
}

void	alloc_cmd_table(t_cmd_table **cmd_list, bool new_cmd)
{
	t_cmd_table	*cmd_table;
	t_cmd_table *last;

	cmd_table = (t_cmd_table *)malloc(sizeof(t_cmd_table));
	if (!cmd_table)
		clean_cmd_table_list(cmd_list);
	last = get_last_cmd_table(*cmd_list);
	if (!(*cmd_list))
	{
		(*cmd_list) = cmd_table;
		cmd_table->prev = NULL;
	}
	else
	{
		last->next = cmd_table;
		cmd_table->prev = last;
	}
	cmd_table->cmd = NULL;
	cmd_table->args = NULL;
	cmd_table->n_args = 0;
	cmd_table->in = 0;
	cmd_table->out = 0;
	cmd_table->err = 0;
	cmd_table->hd_file = NULL;
	cmd_table->next = NULL;
	cmd_table->n_redirections = 0;
	cmd_table->redirections = NULL;
	cmd_table->new_cmd = new_cmd;
}

void	populate_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int nbr_tokens)
{
	int		i;
	int		j;
	int		w;
	char	redir;

	init_pop_cmd_table(&i, &j, &w, &redir, cmd_table);
	while (i < nbr_tokens)
	{
		if (token_list->type == CMD)
			(*cmd_table)->cmd = ft_strdup(token_list->value); 
		else if (token_list->type == ARG)
			(*cmd_table)->args[j++] = ft_strdup(token_list->value);
		else if (token_list->type == PIPE)
			(*cmd_table)->out = PIPE;
		else if ((token_list->type == STDOUT || token_list->type == STDERR) && (w < (*cmd_table)->n_redirections))
			check_std_cmd_table(token_list, cmd_table, w);
		check_redir_cmd_table(token_list, &redir);
		if (token_list->type == FILENAME)
			assign_redir_cmd_table(token_list, cmd_table, &w, redir);
		i++;
		token_list = token_list->next;
	}
	(*cmd_table)->args[j] = NULL;
	(*cmd_table)->n_args = j;
}
