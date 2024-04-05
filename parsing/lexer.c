//#include "minishell.h"
#include "parsing_tests.h"

// 2>errors.txt
// gestionar ;
// gestionar malloc error en add_token
// norminette
// ft para limpiar el array de tokens, justo despues de crear el cmd table. limpiar strings (value) y cada nodo

//		ls -la | cat >output

void	lexer_redir(t_token **token_list, char *cmd_line, int *start, int *i)
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

void	lexer_quotes(t_token **token_list, char *cmd_line, int *start, int i)
{
	if (i > (*start)) 
	{
    	add_token(token_list, cmd_line, (*start), i);
        (*start) = i;
    }
}

void	lexer(char *cmd_line, t_token **token_list)
{
	int 	i;
	int 	start;
	char	quote_type;
	bool	quotes;

	i = 0;
	start = 0;
	quotes = false;
	while (cmd_line[i])
	{
        if (quotes) 
		{
            if (cmd_line[i] == quote_type && (i == 0 || cmd_line[i - 1] != '\\')) 
                quotes = false;
		}
		else 
		{
            if (cmd_line[i] == '"' || cmd_line[i] == '\'') 
			{
                quotes = true;
                quote_type = cmd_line[i];
                lexer_quotes(token_list, cmd_line, &start, i);
            } 
			else if (cmd_line[i] == '<' || cmd_line[i] == '>')
				lexer_redir(token_list, cmd_line, &start, &i);
			else if (ft_isspace(cmd_line[i])) 
			{
                if (i > start) 
					add_token(token_list, cmd_line, start, i);
                start = i + 1;
            }
        }
        i++;
    }
    if (i > start) 
        add_token(token_list, cmd_line, start, i);
}

int	add_token(t_token **token_list, char *cmd_line, int start, int end)
{
	t_token *new_token;
	t_token *last;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (-1);
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
	get_token_type(new_token); 
	return (0);
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
