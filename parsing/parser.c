#include "parsing_tests.h"

// norminette
// probar con inputs no validos a ver como se comporta
// gestionar memoria de lexer y parser con leaks, crear ft y archivo clean.c
// organizar ft en archivos

t_cmd_table *get_last_cmd_table(t_cmd_table *cmd_list)
{
	if (!cmd_list)
		return (0);
	while (cmd_list->next)
		cmd_list = cmd_list->next;
	return (cmd_list);
}
void	init_parser(int *start, int *end, t_token **tmp, t_token **token_list)
{
	*start = 0;
	*end = 0;
	(*tmp) = (*token_list);
}
void	parser(t_cmd_table **cmd_table, t_token **token_list)
{
	t_token		*tmp;
	int 		start;
	int			end;
	bool		new_cmd;

	init_parser(&start, &end, &tmp, token_list);	
	while (tmp)
	{
		(*token_list) = tmp;
		while (*token_list)
		{
			if ((*token_list)->type == PIPE)
			{
				gen_cmd_table(tmp, cmd_table, start, end + 1, new_cmd);
				start = end;
				if (new_cmd)
					new_cmd = false;
			}
			end++;
			(*token_list) = (*token_list)->next; 
		}
		if (*cmd_table && !new_cmd)
			gen_cmd_table(tmp, cmd_table, start + 1, end, new_cmd);
		else
			gen_cmd_table(tmp, cmd_table, start, end, new_cmd);
		tmp = tmp->next_cmd;
		start = 0;
		end = 0;
		new_cmd = true;
	} 
}

int		get_nbr_args(t_token *token_list, int nbr_tokens)
{
	int		i;
	int 	nbr_args;
	t_token	*it;

	i = 0;
	nbr_args = 0;
	it = token_list;
	while (i < nbr_tokens)
	{
		if (it->type == ARG)
			nbr_args++;
		i++;
		it = it->next;
	}
	return (nbr_args);
}
int		get_nbr_redir(t_token *token_list, int nbr_tokens)
{
	int		i;
	int 	nbr_redir;
	t_token	*it;

	i = 0;
	nbr_redir = 0;
	it = token_list;
	while (i < nbr_tokens)
	{
		if (it->type == TRUNC || it->type == APPEND || it->type == INPUT || it->type == HEREDOC)
			nbr_redir++;
		i++;
		it = it->next;
	}
	return (nbr_redir);
}

void	gen_cmd_table(t_token *token_list, t_cmd_table **cmd_list, int start, int end, bool new_cmd)
{
	t_cmd_table	*cmd_table;
	t_cmd_table *last;
	int			i;
	int			j;

	cmd_table = (t_cmd_table *)malloc(sizeof(t_cmd_table));
	if (!cmd_table)
		//ft para limpiar 
	cmd_table = NULL;
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
	i = 0;
	while (i < start)
	{
		token_list = token_list->next;
		i++;
	}
	cmd_table->new_cmd = new_cmd;
	cmd_table->args = (char **)malloc(sizeof(char *) * (get_nbr_args(token_list, (end - start)) + 1));
	cmd_table->n_redirections = get_nbr_redir(token_list, (end - start)); 
	cmd_table->redirections = (t_token **)malloc(sizeof(t_token *) * cmd_table->n_redirections);
	j = 0;
	while (j < cmd_table->n_redirections)
	{
		cmd_table->redirections[j] = (t_token *)malloc(sizeof(t_token));
		j++;
	}
	cmd_table->next = NULL;
	populate_cmd_table(token_list, &cmd_table, (end - start));
}
void	populate_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int nbr_tokens) 
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
			if (token_list->type == STDERR)
				(*cmd_table)->err = token_list->next->type;
			else if (token_list->type == STDOUT)
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
				(*cmd_table)->in = INPUT;
				(*cmd_table)->redirections[w]->type = INPUT;
				(*cmd_table)->redirections[w]->value = ft_strdup(token_list->value);
				w++;
			}
			else if (redir == 'h' && w < (*cmd_table)->n_redirections)
			{
				(*cmd_table)->in = HEREDOC;
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
