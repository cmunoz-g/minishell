#include "minishell.h"

char	**lexer(char *cmd_line)
{
	t_token	*tokens;
	int		i;
	bool	in_quotes;
	char	current_quote_type;

	i = 0;
	in_quotes = false;
	while (cmd_line[i])
	{
		if (in_quotes)
		{

		}
		else 
		{
			if (cmd_line[i] == '"' || cmd_line[i] == '\'')
			{
				in_quotes = true;
				current_quote_type = cmd_line[i];

			}
		}
		
	}


}

void	add_token(t_token *tokens, char *cmd_line, int start, int end)