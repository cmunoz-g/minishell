#include "minishell.h"

void	print_syntax_error(t_token *token)
{
	if (!token)
		printf("minishell: syntax error near unexpected token 'newline'\n");
	else
		printf("minishell: syntax error near unexpected token '%s'\n", token->value);
}

int	check_syntax(t_token *token_list) 
{
	t_token *it;

	it = token_list;
	while (it)
	{
		if (it->type == TRUNC || it->type == INPUT || it->type == APPEND || it->type == HEREDOC)
		{
			if (!it->next)
			{
				print_syntax_error(it->next);
				return (1);
			}
			else if (it->next && !ft_strcmp(it->next->value, "&"))
			{
				if (it->type == APPEND || it->type == HEREDOC)
					print_syntax_error(it->next);
				else
					print_syntax_error(NULL);
				return(1);
			}
			else if (it->next->type != FILENAME)
			{
				print_syntax_error(it->next);
				return (1);
			}
		}
		else if (it->type == PIPE || it->type == DOUBLEPIPE)
		{
			if ((!it->prev && !it->next) || (!it->next))
			{
				print_syntax_error(it);
				return (1);
			}
			if (it->next && (it->next->type != CMD))
			{
				print_syntax_error(it);
				return (1);
			}
		}
		else if (it->type == END)
		{
			if (!it->prev && !it->next)
			{
				print_syntax_error(it);
				return (1);
			}
			if (it->next && (it->next->type == END || it->next->type == EMPTY))
			{
				print_syntax_error(it);
				return (1);
			}
		}
		it = it->next;
	}
	return (0);
}
