
#include "parsing_tests.h"
// syntax pipex  “<infile ls -l | wc -l >outfile”, como se gestiona?
// ft para limpiar el parsed cmd table

t_cmd_table *get_last_cmd_table(t_cmd_table *cmd_list)
{
	if (!cmd_list)
		return (0);
	while (cmd_list->next)
		cmd_list = cmd_list->next;
	return (cmd_list);
}

void	parser(t_cmd_table **cmd_table, t_token **token_list)
{
	t_token		*tmp;
	int 		i;
	bool		new_cmd;

	i = 0;
	tmp = (*token_list);
	while (tmp)
	{
		(*token_list) = tmp;
		while (*token_list)
		{
			if ((*token_list)->type == PIPE)
				gen_cmd_table(*token_list, cmd_table, i, new_cmd);
			if (new_cmd)
				new_cmd = false;
			i = 0;
			(*token_list) = (*token_list)->next; 
		}
		tmp = tmp->next_cmd;
		new_cmd = true;
	} 
}

int		get_nbr_args(t_token *token_list, int nbr_tokens)
{
	int	i;
	int nbr_args;

	i = 0;
	nbr_args = 0;
	while (i < nbr_tokens)
	{
		if (token_list->type == ARG)
			nbr_args++;
		i++;
	}
	return (nbr_args);
}
int		get_nbr_redir(t_token *token_list, int nbr_tokens)
{
	int	i;
	int nbr_args;

	i = 0;
	nbr_args = 0;
	while (i < nbr_tokens)
	{
		if (token_list->type == TRUNC || token_list->type == APPEND || token_list->type == INPUT || token_list->type == HEREDOC)
			nbr_args++;
		i++;
	}
	return (nbr_args);
}

void	gen_cmd_table(t_token *token_list, t_cmd_table **cmd_list, int nbr_tokens, bool new_cmd)
{
	t_cmd_table	*cmd_table;
	t_cmd_table *last;

	cmd_table = (t_cmd_table *)malloc(sizeof(t_cmd_table));
	if (!cmd_table)
		//ft para limpiar 
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
	cmd_table->new_cmd = new_cmd;
	cmd_table->args = (char **)malloc(sizeof(char) * get_nbr_args(token_list, nbr_tokens));
	cmd_table->n_redirections = get_nbr_redir(token_list, nbr_tokens); 
	cmd_table->redirections = (t_token **)malloc(sizeof(t_token *) * cmd_table->n_redirections);	
	populate_cmd_table(token_list, &cmd_table, nbr_tokens);
}
void	populate_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int nbr_tokens) // una vez este terminada, revisar que pasa con la memoria en strdup y demas ft
{
	int		i;
	int		j;
	int		w;
	char	redir;

	i = 0;
	j = 0;
	w = 0;
	redir = '\0';
	if ((*cmd_table)->prev && !(*cmd_table)->new_cmd)
		(*cmd_table)->in = PIPE;
	else
		(*cmd_table)->in = STDIN;
	(*cmd_table)->out = STDOUT;
	(*cmd_table)->err = STDERR;
	while (i < nbr_tokens)
	{
		if (token_list->type == CMD)
			(*cmd_table)->cmd = ft_strdup(token_list->value); 
		else if (token_list->type == ARG)
			(*cmd_table)->args[j++] = ft_strdup(token_list->value);
		else if (token_list->type == PIPE)
			(*cmd_table)->out = PIPE;
		else if ((token_list->type == STDOUT || token_list->type == STDERR) && (w < (*cmd_table)->n_redirections))
		{
			(*cmd_table)->out = token_list->next->type;
			(*cmd_table)->redirections[w]->type = token_list->next->type;
			(*cmd_table)->redirections[w]->value = ft_strdup(token_list->next->next->value);
		}
		else if (token_list->type == TRUNC && token_list->prev->type != STDERR && token_list->prev->type != STDOUT)
			redir = 't';
		else if (token_list->type == APPEND && token_list->prev->type != STDERR && token_list->prev->type != STDOUT)
			redir = 'a';
		else if (token_list->type == INPUT && token_list->prev->type != STDERR && token_list->prev->type != STDOUT)
			redir = 'i';
		else if (token_list->type == HEREDOC && token_list->prev->type != STDERR && token_list->prev->type != STDOUT) 
			redir = 'h';
		else if (token_list->type == FILENAME)
		{
			if (redir == 'i' && w < (*cmd_table)->n_redirections)
			{
				(*cmd_table)->out = INPUT;
				(*cmd_table)->redirections[w]->type = INPUT;
				(*cmd_table)->redirections[w]->value = ft_strdup(token_list->value);
				w++;
			}
			else if (redir == 'h' && w < (*cmd_table)->n_redirections)
			{
				(*cmd_table)->out = HEREDOC;
				(*cmd_table)->redirections[w]->type = HEREDOC;
				(*cmd_table)->redirections[w]->value = ft_strdup(token_list->value);
				w++;
			}
			else if (redir == 't' && w < (*cmd_table)->n_redirections)
			{
				(*cmd_table)->out = TRUNC;
				(*cmd_table)->redirections[w]->type = TRUNC;
				(*cmd_table)->redirections[w]->value = ft_strdup(token_list->value);
				w++;
			}
			else if (redir == 'a' && w < (*cmd_table)->n_redirections)
			{
				(*cmd_table)->out = APPEND;
				(*cmd_table)->redirections[w]->type = APPEND;
				(*cmd_table)->redirections[w]->value = ft_strdup(token_list->value);
				w++;
			}
		}
		i++;
		token_list = token_list->next;
	}
	(*cmd_table)->args[j] = NULL;
}