#include "minishell.h"

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

void	check_std_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int w)
{
	if (token_list->type == STDERR)
		(*cmd_table)->err = token_list->next->type;
	else if (token_list->type == STDOUT)
		(*cmd_table)->out = token_list->next->type;
	(*cmd_table)->redirections[w]->type = token_list->next->type;
	(*cmd_table)->redirections[w]->value = ft_strdup(token_list->next->next->value);
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
void	assign_redir_cmd_table_aux(t_cmd_table **cmd_table, int *w, int type, char *value)
{
	(*cmd_table)->redirections[*w]->type = type;
	(*cmd_table)->redirections[*w]->value = ft_strdup(value);
	(*w)++;
}