#include "minishell.h"

// tener en cuenta las combinaciones que si son posibles de <, >, >>. Por ejemplo, algo que lleve >>>> daria error. Pensar cuando comprobarlo
// esto es algo a revisar. se supone que puedes hacer redirecciones del palo de 1>, 0>, 2>, 2>>. Solo si el numero esta pegado al >. Con 0 y otros numeros el out es stdout. Con 2 es stderr y con 1 el archivo de despues
// MUY IMPORTANTE: si el char * que le paso a lexer es 1 > a.txt no deberia redireccionar como si fuera 1>a.txt.

// Gestionar la definicion de variables por el usuario

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
				lexer_new_cmd(token_list, cmd_line, &i);
				break ;
			}
        }
    }
	(add_token(token_list, cmd_line, start, i), check_redirections(token_list));
}

void	lexer_qt(t_token **token_list, char *cmd_line, int *start, int i)
{
	if (i > (*start)) 
	{
    	add_token(token_list, cmd_line, (*start), i);
        (*start) = i;
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
		if (cmd_line[(*i)] == '>' && cmd_line[(*i) + 1] == '>')
		{
			add_token(token_list, cmd_line, (*i), (*i) + 2);
			(*i)++;
		}
		else if (cmd_line[(*i)] == '<' && cmd_line[(*i) + 1] == '<')
		{
			add_token(token_list, cmd_line, (*i), (*i) + 2);
			(*i)++;
		}
		else 
			add_token(token_list, cmd_line, (*i), (*i) + 1);
		(*start) = (*i) + 1;
	}
}

void	lexer_new_cmd(t_token **token_list, char *cmd_line, int *i)
{
	t_token	*new_cmd;

	new_cmd = NULL;
	lexer((cmd_line + (*i) + 1), &new_cmd);
	(*token_list)->next_cmd = new_cmd;
}

void	check_redirections(t_token **token_list) 
{
	t_token *it_filename;

	it_filename = *(token_list);
	while (it_filename)
	{
		if ((it_filename->next && !ft_strcmp(it_filename->value, "2")
			&& (it_filename->next->type == HEREDOC || it_filename->next->type == TRUNC || it_filename->next->type == APPEND || it_filename->next->type == INPUT)))
			it_filename->type = STDERR;
		if ((it_filename->next && (!ft_strcmp(it_filename->value, "1"))
			&& (it_filename->next->type == HEREDOC || it_filename->next->type == TRUNC || it_filename->next->type == APPEND || it_filename->next->type == INPUT)))
			it_filename->type = STDOUT;
		it_filename = it_filename->next;
	}
}