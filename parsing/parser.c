#include "parsing_tests.h"

// Gestionar memoria lexer y parser, leaks, crear ft y archivo clean.c
// Organizar ft en archivos
// Crear main.c, get line, incorporar en Make, crear una version de minishell ejecutable
// Probar comandos raros e inputs no validos en la parte lexer + parser

t_cmd_table *get_last_cmd_table(t_cmd_table *cmd_list)
{
	if (!cmd_list)
		return (0);
	while (cmd_list->next)
		cmd_list = cmd_list->next;
	return (cmd_list);
}
void	init_parser(int start_end[2], t_token **tmp, t_token **token_list, bool *new_cmd)
{
	start_end[0] = 0;
	start_end[1] = 0;
	(*tmp) = (*token_list);
	*new_cmd = true;
}
void	reset_parser(t_token **tmp, int start_end[2], bool *new_cmd)
{
	(*tmp) = (*tmp)->next_cmd;
	start_end[0] = 0;
	start_end[1] = 0;
	*new_cmd = true;
}

void	parser_aux(t_token **token_list, int *s_e)
{
	s_e[1]++;
	(*token_list) = (*token_list)->next;
}

void	parser(t_cmd_table **cmd_table, t_token **token_list)
{
	t_token		*tmp;
	int			s_e[2];
	bool		new_cmd;

	init_parser(s_e, &tmp, token_list, &new_cmd);	
	while (tmp)
	{
		(*token_list) = tmp;
		while (*token_list)
		{
			if ((*token_list)->type == PIPE)
			{
				(alloc_cmd_table(cmd_table, new_cmd), gen_cmd_table(tmp, cmd_table, s_e[0], s_e[1] + 1));
				s_e[0] = s_e[1];
				if (new_cmd)
					new_cmd = false;
			}
			parser_aux(token_list, s_e);
		}
		if (*cmd_table && !new_cmd)
			(alloc_cmd_table(cmd_table, new_cmd), gen_cmd_table(tmp, cmd_table, s_e[0] + 1, s_e[1]));
		else
			(alloc_cmd_table(cmd_table, new_cmd), gen_cmd_table(tmp, cmd_table, s_e[0], s_e[1]));
		reset_parser(&tmp, s_e, &new_cmd);
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

void	alloc_cmd_table(t_cmd_table **cmd_list, bool new_cmd)
{
	t_cmd_table	*cmd_table;
	t_cmd_table *last;

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
	cmd_table->new_cmd = new_cmd;
}

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
	last->args = (char **)malloc(sizeof(char *) * (get_nbr_args(token_list, (end - start)) + 1));
	last->n_redirections = get_nbr_redir(token_list, (end - start)); 
	last->redirections = (t_token **)malloc(sizeof(t_token *) * last->n_redirections);
	j = 0;
	while (j < last->n_redirections)
	{
		last->redirections[j] = (t_token *)malloc(sizeof(t_token));
		j++;
	}
	last->next = NULL;
	populate_cmd_table(token_list, &last, (end - start));
}

void	init_pop_cmd_table(int *i, int *j, int *w, char *redir, t_cmd_table **cmd_table)
{
	*i = 0;
	*j = 0;
	*w = 0;
	*redir = '\0';
	if ((*cmd_table)->prev && !(*cmd_table)->new_cmd)
		(*cmd_table)->in = PIPE;
	else
		(*cmd_table)->in = STDIN;
	(*cmd_table)->out = STDOUT;
	(*cmd_table)->err = STDERR;
}
void	check_redir_cmd_table(t_token *token_list, char *redir)
{
	if (token_list->type == TRUNC && token_list->prev->type != STDERR && token_list->prev->type != STDOUT)
		*redir = 't';
	else if (token_list->type == APPEND && token_list->prev->type != STDERR && token_list->prev->type != STDOUT)
		*redir = 'a';
	if (token_list->type == INPUT && token_list->prev->type != STDERR && token_list->prev->type != STDOUT)
		*redir = 'i';
	if (token_list->type == HEREDOC && token_list->prev->type != STDERR && token_list->prev->type != STDOUT) 
		*redir = 'h';
}
void	assign_redir_cmd_table_aux(t_cmd_table **cmd_table, int *w, int type, char *value)
{
	(*cmd_table)->redirections[*w]->type = type;
	(*cmd_table)->redirections[*w]->value = ft_strdup(value);
	(*w)++;
}

void	assign_redir_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int *w, char redir)
{
	if (redir == 'i' && *w < (*cmd_table)->n_redirections)
	{
		(*cmd_table)->in = INPUT;
		assign_redir_cmd_table_aux(cmd_table, w, INPUT, token_list->value);
	}
	else if (redir == 'h' && *w < (*cmd_table)->n_redirections)
	{
		(*cmd_table)->in = HEREDOC;
		assign_redir_cmd_table_aux(cmd_table, w, HEREDOC, token_list->value);
	}
	else if (redir == 't' && *w < (*cmd_table)->n_redirections)
	{
		(*cmd_table)->out = TRUNC;
		assign_redir_cmd_table_aux(cmd_table, w, TRUNC, token_list->value);
	}
	else if (redir == 'a' && *w < (*cmd_table)->n_redirections)
	{
		(*cmd_table)->out = APPEND;
		assign_redir_cmd_table_aux(cmd_table, w, APPEND, token_list->value);
	}
}
void	check_std_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int w)
{
	if (token_list->type == STDERR)
		(*cmd_table)->err = token_list->next->type;
	else if (token_list->type == STDOUT)
		(*cmd_table)->out = token_list->next->type;
	(*cmd_table)->redirections[w]->type = token_list->next->type;
	(*cmd_table)->redirections[w]->value = ft_strdup(token_list->next->next->value);
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
}
