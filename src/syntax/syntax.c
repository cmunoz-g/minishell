#include "minishell.h"

// si hay mas de 3, si hay dos diferentes (<, >). si hay solo redirs (o solo redirs entre pipes)


/* SI SOLO HAY REDIRS

camunozg@Carlos:~/minishell$ <
-bash: syntax error near unexpected token `newline'
camunozg@Carlos:~/minishell$ >
-bash: syntax error near unexpected token `newline'
camunozg@Carlos:~/minishell$ <<
-bash: syntax error near unexpected token `newline'
camunozg@Carlos:~/minishell$ >>
-bash: syntax error near unexpected token `newline'


camunozg@Carlos:~/minishell$ >>
-bash: syntax error near unexpected token `newline'
camunozg@Carlos:~/minishell$ >>>
-bash: syntax error near unexpected token `>'
camunozg@Carlos:~/minishell$ >>>>
-bash: syntax error near unexpected token `>>'
camunozg@Carlos:~/minishell$ >>>>>
-bash: syntax error near unexpected token `>>'
camunozg@Carlos:~/minishell$ >>>>>>


camunozg@Carlos:~/minishell$ <<
-bash: syntax error near unexpected token `newline'
camunozg@Carlos:~/minishell$ <<<
-bash: syntax error near unexpected token `newline'
camunozg@Carlos:~/minishell$ <<<<
-bash: syntax error near unexpected token `<'
camunozg@Carlos:~/minishell$ <<<<<
-bash: syntax error near unexpected token `<<'


camunozg@Carlos:~/minishell$ <>>
-bash: syntax error near unexpected token `>'
camunozg@Carlos:~/minishell$ <><
-bash: syntax error near unexpected token `<'
camunozg@Carlos:~/minishell$ <><>
-bash: syntax error near unexpected token `<>'
camunozg@Carlos:~/minishell$ ><
-bash: syntax error near unexpected token `<'
camunozg@Carlos:~/minishell$ ><<
-bash: syntax error near unexpected token `<<'
camunozg@Carlos:~/minishell$ ><>
-bash: syntax error near unexpected token `<>'


camunozg@Carlos:~/minishell$ < |
-bash: syntax error near unexpected token `|'
camunozg@Carlos:~/minishell$ < | >
-bash: syntax error near unexpected token `|'

camunozg@Carlos:~/minishell/src$ |
-bash: syntax error near unexpected token `|'
camunozg@Carlos:~/minishell/src$ ||
-bash: syntax error near unexpected token `||'
camunozg@Carlos:~/minishell/src$ ||||||
-bash: syntax error near unexpected token `||'

camunozg@Carlos:~/minishell/src$ ls <<< ||| >>>>> >
-bash: syntax error near unexpected token `||'
camunozg@Carlos:~/minishell/src$ <<<<<cat || ||| >>>
-bash: syntax error near unexpected token `<<'

*/

void	print_syntax_error(t_token *token)
{
	if (!token)
		printf("-minishell: syntax error near unexpected token 'newline'\n");
	else
		printf("-minishell: syntax error near unexpected troken '%s'\n", token->value);
}

// gestionar multiples ;
// gestionar ls |

int	check_syntax(t_token *token_list) 
{
	t_token *it;
	t_token	*next_cmd;

	it = token_list;
	next_cmd = token_list;

	print_tokens(token_list);
	exit(0);
	while (next_cmd)
	{
		while (it)
		{
			if (it->type == TRUNC || it->type == INPUT || it->type == APPEND || it->type == HEREDOC)
			{
				if (!it->next)
				{
					print_syntax_error(it->next);
					return (1);
				}
				else if (it->next->type != FILENAME)
				{
					print_syntax_error(it->next);
					return (1);
				}
			}
			if (it->type == PIPE || it->type == DOUBLEPIPE)
			{
				if (!it->prev && !it->next)
				{
					print_syntax_error(it);
					return (1);
				}
				if (it->next && (it->next->type != CMD))
				{
					print_syntax_error(it->next);
					return (1);
				}
			}
			it = it->next;
		}
		next_cmd = next_cmd->next_cmd;
	}
	return (0);
}
