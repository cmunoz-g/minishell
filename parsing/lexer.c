//#include "minishell.h"
#include "parsing_tests.h"

// tener en cuenta las combinaciones que si son posibles de <, >, >>. Por ejemplo, algo que lleve >>>> daria error. Pensar cuando comprobarlo

t_token *get_last_token(t_token *token_list)
{
	if (!token_list)
		return (0);
	while (token_list->next)
		token_list = token_list->next;
	return (token_list);
}

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

void	check_redirections(t_token **token_list)
{
	t_token *it_filename;

	it_filename = *(token_list);
	while (it_filename)
	{
		if ((it_filename->next && (it_filename->value[0] == '0' || it_filename->value[0] == '1' || it_filename->value[0] == '2')
			&& (it_filename->next->type == TRUNC || it_filename->next->type == APPEND || it_filename->next->type == INPUT)))
			it_filename->type = FILENAME;
		it_filename = it_filename->next;
	}
}

void	lexer_new_token(t_token **token_list, char *cmd_line, int *start, int *i)
{
	if (ft_isspace(cmd_line[*i]))
	{
		if (*i > *start) 
			add_token(token_list, cmd_line, *start, *i);
   	 	*start = *i + 1;
	}
	else
	{
		if ((*i) > (*start))
			add_token(token_list, cmd_line, (*start), (*i));
		if (cmd_line[(*i) + 1] == '>')
		{
			add_token(token_list, cmd_line, (*i), (*i) + 2);
			(*i)++;
		}
		else 
			add_token(token_list, cmd_line, (*i), (*i) + 1);
		(*start) = (*i) + 1;
	}

}

void	lexer_qt(t_token **token_list, char *cmd_line, int *start, int i)
{
	if (i > (*start)) 
	{
    	add_token(token_list, cmd_line, (*start), i);
        (*start) = i;
    }
}

void	set_qt(bool *quotes, char *quote_type, char *cmd_line, int i)
{
	(*quotes) = true;
	(*quote_type) = cmd_line[i];
}
void	lexer_new_cmd(t_token **token_list, char *cmd_line, int *start, int *i)
{
	t_token	*new_cmd;

	new_cmd = NULL;
	lexer((cmd_line + (*i) + 1), &new_cmd);
	(*token_list)->next_cmd = new_cmd;
}

void	init_lexer(bool *qt, int *i, int *start)
{
	*qt = false;
	*i = -1;
	*start = 0;
}


void	lexer(char *cmd_line, t_token **token_list)
{
	int 	i;
	int 	start;
	char	qt_type;
	bool	qt;

	init_lexer(&qt, &i, &start);
	while (cmd_line[++i])
	{
        if (qt && (cmd_line[i] == qt_type && (i == 0 || cmd_line[i - 1] != '\\'))) 
                qt = false;
		else if (!qt) 
		{
            if (cmd_line[i] == '"' || cmd_line[i] == '\'') 
				(set_qt(&qt, &qt_type, cmd_line, i), lexer_qt(token_list, cmd_line, &start, i));
			else if (ft_isspace(cmd_line[i]) || cmd_line[i] == '<' || cmd_line[i] == '>')
				lexer_new_token(token_list, cmd_line, &start, &i);
			else if (cmd_line[i] == ';')
			{
				lexer_new_cmd(token_list, cmd_line, &start, &i);
				break ;
			}
        }
    }
	(add_token(token_list, cmd_line, start, i), check_redirections(token_list));
}

void	add_token(t_token **token_list, char *cmd_line, int start, int end)
{
	t_token *new_token;
	t_token *last;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		clean_token_list(token_list);
	last = get_last_token(*token_list);
	if (!last)
	{
		*token_list = new_token;
		new_token->prev = NULL;
	}
	else  
	{
		last->next = new_token;
		new_token->prev = last;
	}
	new_token->value = ft_strdup_mod(cmd_line + start, end - start);
	new_token->next = NULL;
	new_token->next_cmd = NULL;
	get_token_type(new_token);
}

void	get_token_type(t_token *token)
{
	if (!ft_strcmp(token->value, ""))
		token->type = EMPTY;
	else if (token->value[0] == '<')
		token->type = INPUT;
	else if (token->value[0] == '>')
		token->type = TRUNC;
	else if (token->value[0] == '>' && token->value[1] == '>')
		token->type = APPEND;
	else if (!ft_strcmp(token->value, "|"))
		token->type = PIPE;
	else if (!ft_strcmp(token->value, ";"))
		token->type = END;
	else if (token->prev && (token->prev->type == TRUNC || token->prev->type == APPEND || token->prev->type == INPUT))
		token->type = FILENAME;
	else if (token->prev == NULL || (token->prev->type >= 6))
		token->type = CMD;
	else
		token->type = ARG;
}
